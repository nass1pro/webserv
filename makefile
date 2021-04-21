# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehafidi <ehafidi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/21 16:24:48 by ehafidi           #+#    #+#              #
#    Updated: 2021/04/21 16:30:39 by ehafidi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =  srcs/main.cpp srcs/printers.cpp \ 
	srcs/request.cpp srcs/server.cpp srcs/utils.cpp srcs/response/response.cpp 

CC = clang++
CCFLAGS = -g -Wall -Wextra -Werror

NAME = webserv
RM = rm -rf

all	: $(NAME) 

$(NAME) :
	$(CC) $(CCFLAGS) $(SRC) -o $(NAME) 

fclean:
	$(RM) *.dSYM
	$(RM) $(NAME)

re: fclean all