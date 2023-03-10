SRCS	=	srcs/main.cpp		\
			srcs/User.cpp	\
			srcs/Server.cpp		\
			srcs/Channel.cpp

HEADER	=	include/Server.hpp	\
			include/User.hpp	\
			include/Colors.hpp	\
			srcs/Channel.cpp	\

OBJS	=	${SRCS:%.cpp=${DIR_OBJ}%.o}

DIR_OBJ	=	./compiled_srcs/

FLAG	=	-Wall -Werror -Wextra -Wshadow -g3

# FLAG_DEBUG	=	-g3

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
