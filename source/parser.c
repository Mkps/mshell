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

	printf ("looking for %s\n", str);
	env = ft_getenv(data->envv, str);
	if (env == NULL)
	{
		free(env);
		return (NULL);
	}
	return (env);
}

// Replaces the string at r_index of length n by str
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

// Replaces the $VAR with its' corresponding value stored in env if it exists.
char	*var_expander(t_data *data, char *str)
{
	int	i;
	int	n;
	
	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			while (*(str + i + n) && ft_get_sep_type(str + i + n) == WORD)
				n++;
			return (var_expander(data, str_replace(str, i, n, get_var(data, ft_str_extract(str + i + 1, n - 1)))));	
		}
		if (str[i] == 92)
		{
			if (str[i + 1] && str[i + 1] == 34)
			 return (var_expander(data, str_replace(str, i, 2, "\"")));
		}
		i++;
	}
	return (str);
}

void	parse_token(t_data *data)
{
	t_token	*current;

	current = *data->token_root;
	while (current != NULL)
	{
		if (current->token_type == WORD && current->quote_status != SQUOTE)
		{
			current->value = var_expander(data, current->value);	
		}
		current = current->next;
	}
}

t_cmd	*create_cmd(t_data *data)
{
	t_cmd	*ret;

	ret = (t_cmd *)malloc(sizeof(t_cmd) * 1);
	if (!ret)
		return (NULL); 
	ret->next = NULL;
	return (ret);
}
void	add_cmd_back(t_data *data)
{
	t_cmd	*last;

	if (*data->cmd_list == NULL)
	{
		*data->cmd_list = create_cmd(data);
		return ;
	}
	last = *data->cmd_list;
	while (last->next != NULL)
		last = last->next;
	last->next = create_cmd(data);
}

int	is_assign(char	*str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while(*str)
	{
		if (*str == '=' && *str + 1)
			return (1); 
		str++;
	}
	return (0);
}
t_cmd	*last_cmd(t_cmd **root)
{
	t_cmd	*current;

	current = *root;
	if (!current)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

int		get_cmd_type(char *value)
{
	if (!value)
		return (EMPTY);
	if (*value == '#')
		return (COMMENT);
	return (CMD);
}

t_token	*add_cmd(t_data *data, t_token *token)
{
	t_cmd	*new_cmd;
	t_token	*current;
	int		type;
	char	*tmp;
	char	**args;

	current = token;
	add_cmd_back(data);
	new_cmd = last_cmd(data->cmd_list);
	new_cmd->cmd = token->value; 
	new_cmd->type = get_cmd_type(token->value);
	new_cmd->fd[0] = -1;
	new_cmd->fd[1] = -1;
	tmp = ";";
	while (current->token_type == WORD )
	{
		tmp = ft_strjoin(tmp, ";");
		tmp = ft_strjoin(tmp, current->value);	
		if (current->next->token_type == OSQUOTE || current->next->token_type == ODQUOTE)
		{
			type = current->next->token_type;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	new_cmd->args = ft_split(tmp, ';');
	// printf("strtmp %s\n", tmp);
	// for (int i = 0; new_cmd->args[i]; i++)
	// 	printf("arg is %s\n", new_cmd->args[i]);
	return (current);
}

void	add_assign_cmd(t_data *data)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(data);
	new_cmd->type = CMD_ASSIGN;
	new_cmd->fd[0] = -1;
	new_cmd->fd[1] = -1;
}
t_token	*get_cmd_first(t_token *current_t)
{
	t_token	*current;

	current = current_t;
	while(current != NULL && current->prev != NULL && current->prev->token_type != PIPE)
		current = current->prev;
	return (current);
}

t_token *get_input_token(t_token *current_t)
{
	t_token	*tmp;
	t_token *input_token;

	input_token = NULL;
	tmp = current_t;
	while(tmp != NULL && tmp->token_type != PIPE)
	{
		if (tmp->token_type == IO_INPUT || tmp->token_type == IO_HEREDOC)
		{	
			input_token = tmp;
			return (input_token);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_token	*get_output_token(t_token *current)
{
	t_token *tmp;
	t_token *output_token;

	output_token = NULL;
	tmp = current;
	while (tmp != NULL && tmp->token_type != PIPE)
	{
		if (tmp != NULL && tmp->token_type != PIPE)
		{
			if (tmp->token_type == IO_TRUNC || tmp->token_type == IO_APPEND)
				output_token = tmp;
			tmp = tmp->next;
		}
	}
	return (output_token);
}

int		set_input_fd(t_token *current)
{
	int	token_type;

	token_type = current->token_type;
	if (token_is_quote(current->next))
		current = current->next;
	if (current->next->token_type != WORD)
		return (-1);
	if (token_type == IO_INPUT)
		return (open_fd(0, current->next->value));
	if (token_type == IO_HEREDOC)
		here_doc_handler(current->next->value);
	return (0);
}
int		set_output_fd(t_token *current)
{
	int	token_type;

	token_type = current->token_type;
	if (current->next->token_type == ODQUOTE)
		current = current->next;
	if (current->next->token_type != WORD)
		return (-1);
	if (token_type == IO_TRUNC)
		return (open_fd(1, current->next->value));
	else if (token_type == IO_APPEND)
		return (open_fd(2, current->next->value));
	return (-1);
}

void	handle_cmd_input(t_data *data, t_token *current_t, t_cmd *cmd)
{
	t_token	*input_token;

	cmd->fd[0] = 0;
	input_token = get_input_token(get_cmd_first(current_t));
	while (input_token != NULL)
	{
		cmd->fd[0] = set_input_fd(input_token);
		printf("received fd is cmd->fd[0] %i\n", cmd->fd[0]);
		input_token = get_input_token(input_token->next);	
		if (cmd->fd[0] < 0)
			return ;
	}
}
void	handle_cmd_output(t_data *data, t_token *current_t, t_cmd *cmd)
{
	t_token	*output_token;

	cmd->fd[1] = 1;
	output_token = get_output_token(get_cmd_first(current_t));
	while (output_token != NULL)
	{
		cmd->fd[1] = set_output_fd(output_token);
		printf("received fd is cmd->fd[1] %i\n", cmd->fd[1]);
		output_token = get_output_token(output_token->next);	
		if (cmd->fd[1] < 0)
			return ;
	}
}
void	handle_cmd_io(t_data *data, t_token *current_t, t_cmd *cmd)
{
	handle_cmd_input(data, current_t, cmd);
	handle_cmd_output(data, current_t, cmd);
}
// Returns the first encountered cmd token
t_token	*get_next_cmd(t_token *src)
{
	t_token	*current;

	current = src;
	while (current != NULL && current->token_type != PIPE)
	{
		printf("test\n");
		if (current != NULL && current->token_type == WORD && !is_assign(current->value))
		{
			if (current->prev == NULL || current->quote_status == NONE && current->prev != NULL && !token_is_io(current->prev) )
			{
				return (current);
			}
			if (current->quote_status != NONE && current->prev != NULL && current->prev->prev != NULL && !token_is_io(current->prev->prev))
			{
				printf("qs is %i !token_is_io %i\n", current->quote_status, !token_is_io(current->prev->prev));
				return (current);
			}
		}
		current = current->next;
	}
	return (NULL);
}
void	set_pipe(t_cmd *cmd)
{
	(void)cmd;
	return ;	
}
void	build_cmd_list(t_data *data, t_token *token)
{
	t_token	*current_t;

	if (token == NULL)
		current_t  = *data->token_root;
	else
		current_t = token;
	while (current_t != NULL)
	{
		// printf("current token type %i | value %s\n", current_t->token_type, current_t->value);
		if (is_assign(current_t->value))
		{
			// add_assign_cmd(data);
			// putenv(current_t->value);
			current_t = current_t->next;
		}
		if ((current_t = get_next_cmd(current_t)) != NULL)
		{
			printf("cmd %s\n", current_t->value);
			printf("test\n");
			current_t = add_cmd(data, current_t);
			handle_cmd_io(data, current_t, last_cmd(data->cmd_list));
			while (!token_is_term(current_t))
				current_t = current_t->next;
			if (current_t && current_t->token_type == PIPE)
				set_pipe(last_cmd(data->cmd_list));
			current_t = current_t->next;
		}
		else
		 	current_t = current_t->next;
	}

}
