.PHONY: all clean 
# эти цели не являются файлами выполняй их даже если соотв файлы существуют
all: assembler processor
# когда запускаем make без цели, то выполняем первую цель после all, то есть записи make stack make all и make эквивалентны

COMP=clang++

CFLAGS_WITH_DEBUG = -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -Wno-c++11-extensions
# CFLAGS = -D _DEBUG
SFML_LIBS := -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system
SFML_CFLAGS := -I/usr/local/include

stack_for_calcul/hash.o: stack_for_calcul/hash.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/log.o: stack_for_calcul/log.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/my_assert.o: stack_for_calcul/my_assert.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/stack_func.o: stack_for_calcul/stack_func.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/main_assemb.o: assembler_task/main_assemb.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/file_work.o: assembler_task/file_work.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/parsing_str.o: assembler_task/parsing_str.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/assembler_struct.o: assembler_task/assembler_struct.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/metki.o: assembler_task/metki.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

processor_task/main_proc.o: processor_task/main_proc.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

processor_task/parse_asm_from_file.o: processor_task/parse_asm_from_file.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

processor_task/processor.o: processor_task/processor.cpp
	$(COMP) $(SFML_CFLAGS) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

processor_task/do_instructions.o: processor_task/do_instructions.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

# Правило для файлов в корневой папке
#%.o: %.cpp
#	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler: assembler_task/main_assemb.o assembler_task/file_work.o assembler_task/parsing_str.o assembler_task/assembler_struct.o assembler_task/metki.o
	$(COMP) -o $@ $^

processor: processor_task/main_proc.o processor_task/parse_asm_from_file.o processor_task/do_instructions.o stack_for_calcul/hash.o stack_for_calcul/log.o stack_for_calcul/my_assert.o stack_for_calcul/stack_func.o processor_task/processor.o
	$(COMP) -o $@ $^ $(SFML_LIBS)

#calc: main.o calcul.o file_work.o parsing_str.o stack_for_calcul/hash.o stack_for_calcul/log.o stack_for_calcul/my_assert.o stack_for_calcul/stack_func.o
#	$(COMP) -o $@ $^

clean:
	rm -f assembler_task/*.o processor_task/*.o stack_for_calcul/*.o *.o assembler processor
# *.p

#clang++ -o assembler assembler_task/*.cpp && ./assembler
#clang++ -o processor processor_task/*.cpp stack_for_calcul/*.cpp && ./processor
#rm -f processor processor_task/*.o stack_for_calcul/*.o *.o
#rm -f assembler assembler_task/*.o

#clang++ -o processor processor_task/*.cpp stack_for_calcul/*.cpp -I/usr/local/include -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system && ./processor