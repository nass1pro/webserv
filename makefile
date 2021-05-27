# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/21 16:24:48 by ehafidi           #+#    #+#              #
#    Updated: 2021/05/25 15:48:48 by ehafidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.cpp srcs/parse_config.cpp srcs/request.cpp srcs/server.cpp srcs/utils.cpp \
		srcs/response/f_respons.cpp  srcs/printers.cpp srcs/cgi.cpp srcs/parse_url.cpp

CC = clang++
CCFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv
RM = rm -rf

all	: $(NAME)

$(NAME) :
	$(CC) $(CCFLAGS) $(SRC) -o $(NAME)

fclean: clean
	$(RM) *.dSYM

clean:
	$(RM) $(NAME)

re: fclean clean all
