/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hde-souz <hde-souz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 21:17:55 by hde-souz          #+#    #+#             */
/*   Updated: 2023/11/20 23:55:57 by hde-souz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*	parse_line will handle the concatenation of the buffer and the line
 *	by allocating a new string and copying the previous line and the buffer
 *	into it, then freeing the previous line and returning the new one
*/
char	*parse_line(char *new_line, char *buffer, int buff_size, int *line_len)
{
	char	*line;

	auto int index = 0;
	auto int size_line = 0;
	auto int buffer_index = 0;
	line = new_line;
	*line_len += buff_size;
	new_line = malloc(*line_len + 1 * sizeof(char));
	if (new_line)
	{
		new_line[*line_len] = '\0';
		while (line && line[size_line])
			new_line[index++] = line[size_line++];
		size_line = 0;
		while (buffer && buffer[size_line])
		{
			if (size_line < buff_size)
				new_line[index++] = buffer[size_line];
			else
				buffer[buffer_index++] = buffer[size_line];
			buffer[size_line++] = 0;
		}
	}
	free(line);
	return (new_line);
}

/*	Reads and processes data from the file into the static buffer
	Updates the size of the buffer after each read operation.
	Flags are used to track the progress of the read and to identify '\n'
	Returns a dynamically allocated string containing the next line
*/

char	*read_bf(int fd, char buffer[][BUFFER_SIZE + 1], int *sz_bf)
{
	auto int flag = 1;
	auto int size_line = 0;
	auto char *line = NULL;
	while (flag > 0)
	{
		if (!buffer[fd][0])
			*sz_bf = read(fd, buffer[fd], BUFFER_SIZE);
		flag = *sz_bf;
		if (*sz_bf > 0)
		{
			*sz_bf = 0;
			while (buffer[fd][*sz_bf] && buffer[fd][*sz_bf] != '\n')
				(*sz_bf)++;
			flag = (flag == *sz_bf);
			*sz_bf += buffer[fd][*sz_bf] == '\n';
			line = parse_line(line, buffer[fd], *sz_bf, &size_line);
		}
		else if (*sz_bf == -1)
		{
			free(line);
			line = NULL;
		}
	}
	return (line);
}

/*	Reads a line from a file specified by the file descriptor fd
	Uses a static buffer to store and concatenate data read from the file
	Returns a dynamically allocated str containing the next line of the fd
*/

char	*get_next_line(int fd)
{
	static char	buffer[FOPEN_MAX][BUFFER_SIZE + 1];

	auto int sz_bf = 0;
	if ((fd < 0 || fd > FOPEN_MAX))
		return (NULL);
	while (buffer[fd][sz_bf])
		sz_bf++;
	return (read_bf(fd, buffer, &sz_bf));
}

/*testing multiple files
int main(int argc, char **argv)
{
    char *line;

    if (argc >= 3)
    {
        for (int i = 1; i < argc; i++)
        {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1)
            {
                perror("Error opening file");
		continue;
            }
            while ((line = get_next_line(fd)))
            {
                printf("%s", line);
                free(line);
            }
            close(fd);
        }
    }
    else
        fprintf(stderr, "Wrong call! Try: %s file1 file2 file3 ...\n", argv[0]);
    return 0;
} */

//testing single call of get_next_line
/* int main(int argc, char **argv)
{
    if (argc == 2)
    {
        int fd = open(argv[1], O_RDONLY);

        char *line = get_next_line(fd);

        if (line != NULL)
        {
                printf("%s", line);
                free(line);
        }
	get_next_line(fd);
	printf("%s", line);
    }
    else
    {
        fprintf(stderr, "Wrong call! Try: %s file1 \n", argv[0]);
        return 1;
    }
    return 0;
} */
