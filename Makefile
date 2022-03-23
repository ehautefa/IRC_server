SRCS	=	srcs/main.cpp		\
			srcs/checkArg.cpp	\

HEADER	=	include/serv.hpp	\
			include/Command.hpp	\
			include/Colors.hpp

OBJS	=	${SRCS:%.cpp=${DIR_OBJ}%.o}

DIR_OBJ	=	./compiled_srcs/

FLAG	=	-Wall -Werror -Wextra -Wshadow

FLAG_DEBUG	=	-fsanitize=address -g3

NAME	=	ircserv

CC		=	c++ -std=c++98

RM		= 	rm -rf

${NAME}:	${OBJS} ${HEADER}
			${CC} ${FLAG} ${OBJS} -o ${NAME}

${OBJS}:			| ${DIR_OBJ}

${DIR_OBJ}:
				mkdir -p ${DIR_OBJ}
				mkdir -p ${DIR_OBJ}/srcs

${DIR_OBJ}%.o:%.cpp		
						${CC} ${FLAG} -c $< -o ${<:%.cpp=${DIR_OBJ}%.o}


debug:		${OBJS} ${HEADER}
			${CC} ${FLAG} ${FLAG_DEBUG} ${OBJS} -o ${NAME}

all:		${NAME}

clean:
			${RM} ${DIR_OBJ}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
