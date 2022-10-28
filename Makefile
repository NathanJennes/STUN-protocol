MAKEFLAGS	+=		-rR

NAME		:=		stun

CXX			:=		g++
CXX_FLAGS	:=		-Wall -Wextra -MD -std=c++17 -fsanitize=address -g
LD_FLAGS	:=		-fsanitize=address

SRCS_DIR	:=		src
OBJS_DIR	:=		objs

SRCS		:=		main.cpp \
					util/NetworkByteBuffer.cpp \
					Stun/BindRequest.cpp Stun/Response.cpp Stun/stun_client.cpp
OBJS		:=		$(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.o))
DEPENDS		:=		$(addprefix $(OBJS_DIR)/, $(SRCS:.cpp=.d))

.PHONY: all
all: $(NAME)

.PHONY: run
run: $(NAME)
	./$(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CXX) $(LD_FLAGS) $(OBJS) -o $(NAME)

info:
	@echo $(SRCS)
	@echo $(OBJS)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: clean
clean:
	rm -rf $(OBJS_DIR)

.PHONY: re
re: fclean all

-include $(DEPENDS)
