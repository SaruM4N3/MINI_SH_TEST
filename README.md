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

#### Code complet

<details>
<summary><b>BEFORE (click to expand)</b></summary>

```c
char	**build_envp_tab_from_lst_env(t_env *env)
{
	t_env	*curr;
	int		i;
	char	**envp;
	char	*key_equal;

	i = 0;
	curr = env;
	envp = malloc(sizeof(char *) * (count_node(curr) + 1));
	if (!envp)
		return (NULL);
	while (curr)
	{
		if (curr->key && curr->value == NULL)
			envp[i] = ft_strdup(curr->key);
		else if (curr->value != NULL)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(curr->key, "=");
		}
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		key_equal = envp[i];  // ❌ Pointeur direct
		if (curr->value != NULL)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(key_equal, curr->value);  // ❌ Use-after-free
		}
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		curr = curr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
```
</details>

<details>
<summary><b>AFTER (click to expand)</b></summary>

```c
char	**build_envp_tab_from_lst_env(t_env *env)
{
	t_env	*curr;
	int		i;
	char	**envp;
	char	*key_equal;

	i = 0;
	curr = env;
	envp = ft_calloc(sizeof(char *), (count_node(curr) + 1));  // ✅ ft_calloc
	if (!envp)
		return (NULL);
	while (curr)
	{
		if (curr->key && curr->value == NULL)
			envp[i] = ft_strdup(curr->key);
		else if (curr->value != NULL)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(curr->key, "=");
		}
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		key_equal = ft_strdup(envp[i]);  // ✅ Copie indépendante
		if (curr->value != NULL)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(key_equal, curr->value);  // ✅ OK
			free(key_equal);  // ✅ Free la copie
		}
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		curr = curr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
```
</details>

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

<details>
<summary><b>BEFORE (click to expand)</b></summary>

```c
static void	process_expansion(const char *str, size_t *i, size_t *j,
		t_expand_data *data)
{
	if (str[*i] == '\\' && str[*i + 1] == '$')
	{
		(*data->result)[(*j)++] = '$';
		(*i) += 2;
	}
	else if (str[*i] == '$')
	{
		if (!handle_dollar_sign(str, i, data))
		{
			free(*data->result);
			*data->result = NULL;
			return ;
		}
	}
}

static void	handle_mixed_quotes(const char *str, size_t *i)
{
	if ((str[*i] == '"' && str[*i + 1] == '"') || (str[*i] == '\'' && str[*i
			+ 1] == '\''))
	{
		(*i) += 2;
		return ;
	}
	if (str[*i] == '"' || str[*i] == '\'')
	{
		(*i)++;
		return ;
	}
}

static void	handle_expansion(const char *str, size_t *i, size_t *j,
		t_expand_data *data)
{
	char	*new_result;

	handle_mixed_quotes(str, i);
	process_expansion(str, i, j, data);  // ❌ Incrémente *i
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
	(*data->result)[(*j)++] = str[(*i)++];  // ❌ Copie quand même !
}
```
</details>

<details>
<summary><b>AFTER (click to expand)</b></summary>

```c
static void	handle_mixed_quotes(const char *str, size_t *i)
{
	if ((str[*i] == '"' && str[*i + 1] == '"') || (str[*i] == '\'' && str[*i
			+ 1] == '\''))
	{
		(*i) += 2;
		return ;
	}
	if (str[*i] == '"' || str[*i] == '\'')
	{
		(*i)++;
		return ;
	}
}

// ✅ process_expansion() supprimée - logique intégrée directement

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
</details>

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

---

## 🎨 Format et Outils

### Pourquoi Markdown ?

- **Lisibilité** : Structure claire avec headers, listes, code blocks
- **Portable** : Compatible GitHub, VSCode, Obsidian, etc.
- **Coloration syntaxique** : Code coloré automatiquement
- **Convertible** : Peut être exporté en HTML/PDF facilement

### Outils recommandés

**Éditeurs :**
- VSCode (avec Markdown Preview)
- Obsidian
- Typora
- VIM/EMACS (avec markdown-mode)

**Conversion :**
```bash
# Markdown → PDF
pandoc -f markdown -o notes.pdf minishell-debug.md

# Markdown → HTML
pandoc -f markdown -o notes.html minishell-debug.md

# Markdown → DOCX
pandoc -f markdown -o notes.docx minishell-debug.md
```

---

## ✅ Checklist de Debug

- [ ] Règles Makefile `debug` et `fdebug` ajoutées
- [ ] Macro `DEBUG_MODE` définie dans `minishell.h`
- [ ] Fix du conditional jump dans `update_envp_exec.c`
- [ ] Fix de l'invalid read dans `expander.c`
- [ ] Exit code corrigé (retourne 0 au lieu de 1)
- [ ] Tests avec Valgrind : `make fdebug`
- [ ] Vérification de tous les `malloc` → remplacer par `ft_calloc` si nécessaire

---

*Dernière mise à jour : 2025-11-06*