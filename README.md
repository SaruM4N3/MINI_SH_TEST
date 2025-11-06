# Minishell - Notes de Debug et Fixes

## 📋 Ajouts QoL (Quality of Life)

### Makefile

Ajoute ces règles pour faciliter le debug avec Valgrind :

```makefile
# Compile for valgrind
debug: CFLAGS += -g3
debug: fclean all
	valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --suppressions=readline.supp \
		--trace-children=yes ./minishell

# Compile for debug mode + valgrind
fdebug: CFLAGS += -g3 -DDEBUG_MODE=1
fdebug: fclean all
	DEBUG_MODE=1 valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --suppressions=readline.supp \
		--trace-children=yes ./minishell
```

### minishell.h

Ajoute cette macro pour activer/désactiver les prints de debug :

```c
#ifndef DEBUG_MODE
# define DEBUG_MODE 0
#endif
```

### Utilisation dans les fichiers *.c

Conditionne tes `printf` de debug avec `DEBUG_MODE` :

```c
if (DEBUG_MODE)
{
	print_cmds(data.cmds);
	// print_lst_env(envd);
}
```

---

## 🐛 Problèmes Mineurs

### Exit code incorrect
- **Problème** : Exit code `1` au lieu de `0` quand on exit directement
- **À fixer** : Vérifier la valeur de retour dans la fonction `exit`

---

## 🔥 Problèmes MAJEURS

### 1. Conditional Jump - Invalid Free

#### Test
```bash
# Relancer minishell dans minishell
./minishell
Minishell> ./minishell
```

#### Source
- **Fichier** : `update_envp_exec.c`
- **Fonction** : `build_envp_tab_from_lst_env(t_env *env)`
- **Lignes problématiques** :
  - `53: envp = malloc(sizeof(char *) * (count_node(curr) + 1));`
  - `70: key_equal = envp[i];`

#### Explication du bug
```c
key_equal = envp[i];  // ❌ key_equal pointe sur envp[i]

if (curr->value != NULL)
{
	free(envp[i]);  // ❌ On free envp[i]
	envp[i] = ft_strjoin(key_equal, curr->value);  // ❌ key_equal est invalide !
}
```

**Problème** : `key_equal` n'est pas une copie, c'est un pointeur vers `envp[i]`. Quand on `free(envp[i])`, `key_equal` pointe sur de la mémoire libérée → **use-after-free**.

#### Fix

**Ligne 53** :
```c
// BEFORE
envp = malloc(sizeof(char *) * (count_node(curr) + 1));

// AFTER
envp = ft_calloc(sizeof(char *), (count_node(curr) + 1));
```

**Ligne 70** :
```c
// BEFORE
key_equal = envp[i];

// AFTER
key_equal = ft_strdup(envp[i]);  // ✅ Copie indépendante
// ...
if (curr->value != NULL)
{
	free(envp[i]);
	envp[i] = ft_strjoin(key_equal, curr->value);
	free(key_equal);  // ✅ N'oublie pas de free la copie !
}
```

---

### 2. Invalid Read - Buffer Overflow

#### Test
```bash
Minishell> echo $?
# ==409638== Invalid read of size 1
```

#### Source
- **Fichier** : `expander.c`
- **Fonction** : `handle_expansion(...)`
- **Ligne** : `90: (*data->result)[(*j)++] = str[(*i)++];`

#### Explication du bug

Avec `str = "$?"` (3 bytes: `'$'`, `'?'`, `'\0'`) :

1. `handle_expansion()` appelle `process_expansion()`
2. `process_expansion()` détecte `'$'` → appelle `handle_dollar_sign()`
3. `handle_dollar_sign()` incrémente `*i` : `0 → 1` (skip `'$'`)
4. `handle_variable_expansion()` détecte `'?'` → incrémente `*i` : `1 → 2`
5. Retour dans `handle_expansion()`
6. **Ligne 90** lit `str[2]` (le `'\0'`) et incrémente `*i → 3`
7. **Prochaine itération** : `str[3]` → **LECTURE HORS DU BUFFER** ❌

**Problème** : Double consommation du caractère → `process_expansion()` incrémente `*i`, puis `handle_expansion()` le copie quand même.

#### Fix

**Principe** :
- Si c'est un **cas spécial** (`$`, `\$`) → traite ET `return` (pas de copie)
- Si c'est un **caractère normal** → copie seulement
- **Jamais les deux en même temps**

```c
static void	handle_expansion(const char *str, size_t *i, size_t *j,
		t_expand_data *data)
{
	char	*new_result;

	handle_mixed_quotes(str, i);

	// ✅ Cas spéciaux - return après traitement
	if (str[*i] == '\\' && str[*i + 1] == '$')
	{
		(*data->result)[(*j)++] = '$';
		(*i) += 2;
		return;  // ✅ Pas de copie après
	}
	else if (str[*i] == '$')
	{
		if (!handle_dollar_sign(str, i, data))
		{
			free(*data->result);
			*data->result = NULL;
		}
		return;  // ✅ Pas de copie après
	}

	// ✅ Cas régulier - copie seulement si pas d'expansion
	if (str[*i])
	{
		if (*j >= *(data->result_size) - 1)
		{
			*(data->result_size) *= 2;
			new_result = ft_realloc(*data->result, *(data->result_size));
			if (!new_result)
			{
				free(*data->result);
				*data->result = NULL;
				return ;
			}
			*data->result = new_result;
		}
		(*data->result)[(*j)++] = str[(*i)++];  // ✅ Copie seulement ici
	}
}
```

---

### 3. Crash dans init_envp - NULL value

#### Test
```bash
Minishell> export a
Minishell> export
Error
Malloc fail in init_envp 3
```

#### Source
- **Fichier** : `env.c`
- **Fonction** : `init_envp(t_data *data)`
- **Lignes problématiques** :
  - `73-74: str = ft_strjoin(tmp->key, "=");`
  - `75: data->envp[i] = ft_strjoin(str, tmp->value);`

#### Explication du bug

Quand tu fais `export a` (sans `=`) :
- `tmp->key = "a"`
- `tmp->value = NULL` ❌

**Dans `init_envp()`** :
```c
str = ft_strjoin(tmp->key, "=");      // str = "a="
data->envp[i] = ft_strjoin(str, tmp->value);  // ❌ ft_strjoin("a=", NULL) → CRASH
```

**Problème** : `ft_strjoin()` ne gère pas `NULL` comme second argument → segfault

#### Fix

Vérifier si `tmp->value` est `NULL` avant de construire la string :

```c
void	init_envp(t_data *data)
{
	t_env	*tmp;
	char	*str;
	int		i;

	i = count_env(data);
	if (data->envp)
		free_envp_at_init(data->envp);
	data->envp = ft_calloc((i + 1), sizeof(char *));
	if (!data->envp)
		free_all(data, 0, "Error\nMalloc fail in init_envp 1\n");
	i = 0;
	tmp = data->env;
	while (tmp)
	{
		if (tmp->value == NULL)  // ✅ Si pas de valeur, juste la clé
		{
			data->envp[i] = ft_strdup(tmp->key);
		}
		else  // ✅ Si valeur, construire "key=value"
		{
			str = ft_strjoin(tmp->key, "=");
			if (!str)
				free_all(data, 0, "Error\nMalloc fail in init_envp 2\n");
			data->envp[i] = ft_strjoin(str, tmp->value);
			free(str);
		}
		if (!data->envp[i])
			free_all(data, 0, "Error\nMalloc fail in init_envp 3\n");
		i++;
		tmp = tmp->next;
	}
	data->envp[i] = NULL;
}
```

**Ce qui change** :
- Si `tmp->value == NULL` → juste copier `tmp->key` (pour `export a`)
- Si `tmp->value != NULL` → construire `"key=value"` (normal)
- Évite d'appeler `ft_strjoin(str, NULL)` qui crash ✅

---

### 4. Memory Leak - Key not freed when updating existing env

#### Test
```bash
Minishell> export a=123
Minishell> export a=456
Minishell> env
# Valgrind détecte un leak de 4 bytes
==478736== 4 bytes in 2 blocks are definitely lost in loss record 2 of 66
==478736==    at 0x406151: ft_strndup (ft_strndup.c:23)
==478736==    by 0x4052C4: split_key_value (ft_export_utils.c:22)
```

#### Source
- **Fichier** : `ft_export_utils.c`
- **Fonction** : `try_update_existing_env(t_env *env, const char *key, char *value)`
- **Lignes problématiques** :
  - `45-51: if (value) { ... } else { ... }`

#### Explication du bug

```c
static bool try_update_existing_env(t_env *env, const char *key, char *value)
{
	t_env   *tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (value)  // ✅ Valeur existante
			{
				free(tmp->value);
				tmp->value = value;
				// ❌ MAIS key n'est JAMAIS free !
			}
			else  // ❌ Pas de valeur
			{
				free((char *)key);  // ✅ Free key ici
			}
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}
```

**Problème** : Quand on update une variable existante avec une nouvelle valeur, le `key` alloué dans `split_key_value()` n'est jamais free. Le `free((char *)key)` n'est que dans le `else`, donc il ne s'exécute que si `value == NULL`.

#### Scénario du leak

1. `export a=123` → crée nouvelle variable `a` avec valeur `123`
2. `export a=456` → appelle `split_key_value("a=456", &key, &value)`
   - `key = "a"` (alloué via `ft_strndup`)
   - `value = "456"` (alloué via `ft_strdup`)
3. `try_update_existing_env()` trouve `a` existant
4. `if (value)` → **TRUE** → update `tmp->value` avec `"456"`
5. **MAIS `key` n'est JAMAIS free !** → **LEAK de 1 byte ("a") × 2 fois = 4 bytes** ❌

#### Fix

Le `free((char *)key)` doit être **en dehors du if/else** pour être **toujours exécuté** :

```c
static bool try_update_existing_env(t_env *env, const char *key, char *value)
{
	t_env   *tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = value;
			}
			free((char *)key);  // ✅ TOUJOURS free, qu'il y ait value ou pas
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}
```

**Ce qui change** :
- Le `free((char *)key)` est maintenant **inconditionnel**
- S'exécute même quand `value != NULL` ✅
- Plus de leak ! ✅

---

## 📚 Règles Générales (parce que c'est la merde)

### Préférer `ft_calloc` à `malloc`

**Pourquoi ?** Évite les **invalid free** et les **conditional jumps** quand tu fais des `free` de bâtards.

```c
// ❌ MAUVAIS
envp = malloc(sizeof(char *) * (count + 1));
key_equal = envp[i];  // Pointeur direct
free(envp[i]);        // key_equal est maintenant invalide !

// ✅ BON
envp = ft_calloc(sizeof(char *), (count + 1));
key_equal = ft_strdup(envp[i]);  // Copie indépendante
free(envp[i]);                   // OK
free(key_equal);                 // N'oublie pas de free !
```

### Toujours dupliquer avant free

Si tu dois free un pointeur dans une boucle ou une condition, **duplie-le d'abord** :

```c
// ❌ MAUVAIS
key_equal = envp[i];
free(envp[i]);
envp[i] = ft_strjoin(key_equal, curr->value);  // ❌ Use-after-free

// ✅ BON
key_equal = ft_strdup(envp[i]);  // Copie indépendante
free(envp[i]);
envp[i] = ft_strjoin(key_equal, curr->value);  // ✅ OK
free(key_equal);  // N'oublie pas !
```

### Vérifier les NULL avant ft_strjoin

`ft_strjoin()` n'aime pas les arguments `NULL` :

```c
// ❌ MAUVAIS
if (value != NULL)
{
	result = ft_strjoin(key, value);  // OK si value != NULL
}
// Mais si value == NULL, on ne fait rien ❌

// ✅ BON
if (value == NULL)
{
	result = ft_strdup(key);  // Juste la clé
}
else
{
	str = ft_strjoin(key, "=");
	result = ft_strjoin(str, value);  // "key=value"
	free(str);
}
```

### Toujours free les variables temporaires allouées

Quand tu alloues une variable temporaire (comme `key` dans `split_key_value()`), assure-toi qu'elle sera toujours free, peu importe le chemin d'exécution :

```c
// ❌ MAUVAIS
if (condition)
{
	free(key);  // Seulement ici
}
return (true);  // key non free si condition est FALSE

// ✅ BON
// Free key TOUJOURS, peu importe la condition
free(key);
return (true);
```

---

## ✅ Checklist de Debug

- [ ] Règles Makefile `debug` et `fdebug` ajoutées
- [ ] Macro `DEBUG_MODE` définie dans `minishell.h`
- [ ] Fix du conditional jump dans `update_envp_exec.c` (Problème #1)
- [ ] Fix de l'invalid read dans `expander.c` (Problème #2)
- [ ] Fix du crash dans `init_envp()` pour variables sans valeur (Problème #3)
- [ ] Fix du memory leak dans `try_update_existing_env()` (Problème #4)
- [ ] Exit code corrigé (retourne 0 au lieu de 1)
- [ ] Tests avec Valgrind : `make fdebug`
- [ ] Vérification de tous les `malloc` → remplacer par `ft_calloc` si nécessaire
- [ ] Vérification de toutes les fonctions ft_strjoin pour NULL values
- [ ] Vérification que tous les free sont **inconditionnels** ou **systématiques**

---

*Dernière mise à jour : 2025-11-06 04:22*
