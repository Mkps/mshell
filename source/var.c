/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <aloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 22:50:23 by aloubier          #+#    #+#             */
/*   Updated: 2023/09/22 16:57:41 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

char	*str_env(t_data *data, char *str)
{
	int		i;
	char	*tmp;
	char	**env;

 tmp = 0;
	i = 0;
	env = data->envv;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}
//Returns the value of the var. NULL if not set.
char	*get_var(t_data *data,char *str)
{
	int		i;
	char	*ret;
	char	*env;
	char	**env_p;
	char	*tmp;

	env = ft_getenv(data->envv, str);
	if (env == NULL)
	{
		free(env);
		return (NULL);
	}
	return (env);
}

char	*str_replace(char *src, int r_index, int n, char *str)
{
	int	i;
	int		ret_len;
	int		str_len;
	int		last_index;
	char	*ret;
	
	if (str == NULL)
		str_len = 0;
	else
		str_len = ft_strlen(str);
	last_index = r_index + n;
	if (last_index > ft_strlen(src))
		last_index = ft_strlen(src);
	ret_len = ft_strlen(src) + (last_index - r_index) + str_len; 
	ret = (char *)malloc(sizeof (char) * (ret_len + 1));
	if (!ret)
		printf("error allocating mem for return string\n");
	i = 0;
	while (i < r_index)
	{
		ret[i] = src[i];
		i++;
	}
	while (str && *str)
		ret[i++] = *str++;
	while (i < ret_len && src[last_index])
		ret[i++] = src[last_index++];
	ret[i] = 0;
	return (ret);
}

int		is_valid_var(char *str)
{
	int	i;

	if (*str == 0 || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	i = 0;
	while(str[i])
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		i++;
	}
	return (1);
}

int	var_is_multiple(char *var)
{
	int	i;
	int	ws;

	i = -1;
	while (var[++i])
		if (var[i] == ' ')
			return (1);
	return (0);
}

int	retokenize(t_data *data, char *str, t_token *token)
{
	char	**split;
	t_token	*new_token;
	t_token	*current;
	int		i;
	
	current = token;
	split = ft_split(str, ' ');
	if (!split)
		return (EXIT_FAILURE);
	token->value = split[0];
	i = 1;
	while (split[i])
	{
		insert_token_next(current, WORD, ft_strdup(split[i]));
		current = current->next;
		i++;
	}
	return (EXIT_SUCCESS);
}

// Replaces the $VAR with its' corresponding value stored in env if it exists.
int	var_expander(t_data *data, char *str, t_token *token)
{
	int		i;
	int		n;
	char	*ret;
	char	*tmp;
	char	*var_id;
	char	*tmp_str;
	char	*exit_code;
	int		flag_retokenize;

	flag_retokenize = 1;
	i = 0;
	n = 1;
	ret = ft_strdup(str);
	exit_code = ft_itoa(g_exit_code);
	while (ret[i])
	{
		if (ret[i] == '$')
		{
			if (ft_isalpha(ret[i + 1]) || ret[i + 1] == '_')
			{
				n = 1;
				while (*(ret + i + n) && (ft_isalnum(ret[i + n]) || ret[i + n] == '_'))
					n++;
				if (n != 1)
				{
					tmp = ret;
					tmp_str = ft_str_extract(ret + i + 1, n - 1);
					var_id = get_var(data,tmp_str);
					if (var_is_multiple(var_id))
						flag_retokenize++;
					ret = str_replace(ret, i, n, get_var(data, tmp_str));
					free(tmp_str);
					free(tmp);
				}
				i = 0;
			}
			else if (ft_isdigit(ret[i + 1]))
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, "");
				free(tmp);
				i = 0;
			}
			else if ((ret[i + 1] == 0) && token->near_quote == 1)
			{
				ret[i] = 0;
			}
			else if (ret[i + 1] == '?')
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, exit_code);
				free(tmp);
				i = 0;
			}
			else
				i++;
		}
		else if (ret[i] == 92)
		{
			if (ret[i + 1] && ret[i + 1] == 92)
			{
				tmp = ret;
				ret = str_replace(ret, i, 2, "\\");
				free(tmp);
			}
			i++;
		}
		else
			i++;
	}
	free(exit_code);
	free(str);
	if (flag_retokenize > 0 && token->quote_status == NONE)
	{
		if (retokenize(data, ret, token) == EXIT_SUCCESS)
			return (EXIT_SUCCESS);
	}
	token->value = ret;
	return (flag_retokenize != 0);
}

