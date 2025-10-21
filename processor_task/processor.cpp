#include "processor.h"
#include "parse_asm_from_file.h"
#include "../stack_for_calcul/stack_func.h"
#include "../stack_for_calcul/mistakes_code.h"
#include "../stack_for_calcul/log.h"
#include "color_lib_proc.h"
// #include "SFML/Graphics.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

processor init(const char* name_of_file){
    processor baikal = {};

    if(!name_of_file){
        fprintf(stderr, "NULL pointer to string with file name");
        return baikal;
    }

    baikal.code = load_code(name_of_file);
    if(!baikal.code.comands){
        fprintf(stderr, "NULL pointer to bytecode array");
        return baikal;
    }

    baikal.stack = stack_ctor(baikal.code.size, __FILE__, __func__, __LINE__);
    if(!baikal.code.comands){
        fprintf(stderr, "Can't allocate stack memory to stackfor bytecode");
        processor_free(&baikal);
        return baikal;
    }

    baikal.call_stack = stack_ctor(baikal.code.size, __FILE__, __func__, __LINE__);
    if(!baikal.code.comands){
        fprintf(stderr, "Can't allocate stack memory to stack");
        processor_free(&baikal);
        return baikal;
    }

    baikal.ram_counter = 0;
    baikal.ic = 0;
    return baikal;
}

void bytecode_dump(processor* intel){
    printf_to_log_file( PURPLE "Num of elements in bytecode array: %lu\n" COLOR_RESET, intel->code.size);
    for(size_t idx = 0; idx < intel->code.size; idx++){
        if(intel->ic == idx){
            printf_to_log_file(PINK_DARK "--> %d " COLOR_RESET, intel->code.comands[idx]);
        }
        printf_to_log_file(PURPLE_LIGHT "%d " COLOR_RESET, intel->code.comands[idx]);
    }
}

void reg_dump(processor* intel){
    printf_to_log_file(ORANGE "\nRegister max size: %d\n" COLOR_RESET, REGISTR_MAX_SIZE);
    for(size_t idx_reg = 0; idx_reg < REGISTR_MAX_SIZE; idx_reg++){
        if (idx_reg % 2 == 0) {
        printf_to_log_file(ORANGE_LIGHT "R%cX: %d\t" COLOR_RESET, idx_reg + 'A', intel->registr[idx_reg]);
        }
        else{
        printf_to_log_file(ORANGE_DARK "R%cX: %d\t" COLOR_RESET, idx_reg + 'A', intel->registr[idx_reg]);
        }

        if((idx_reg + 1) % 4 == 0){
            printf_to_log_file("\n");
        }
    }
}

void ram_dump(processor* intel){
    printf_to_log_file(BLUE "\nRAM max size %d\n" COLOR_RESET, RAM_MAX_SIZE);
    for(size_t idx_ram = 0; idx_ram < RAM_MAX_SIZE; idx_ram++){
        if (idx_ram % 2 == 0) {
        printf_to_log_file(BLUE_LIGHT "%d\t" COLOR_RESET, intel->RAM[idx_ram]);
        } 
        else {
        printf_to_log_file(BLUE_DARK "%d\t" COLOR_RESET, intel->RAM[idx_ram]);
        }

        if((idx_ram + 1) % 10 == 0){
            printf_to_log_file("\n");
        }
    }
}

void processor_dump(processor* intel){
    bytecode_dump(intel);

    reg_dump(intel);

    ram_dump(intel);

    printf_to_log_file(GREEN_ELECTRIC "Calcul stack:\n" COLOR_RESET);
    stack_dump(intel->stack);

    printf_to_log_file(GREEN_ELECTRIC "Call stack:\n" COLOR_RESET);
    stack_dump(intel->call_stack);
}

stack_err_bytes processor_verify(processor* intel){
    stack_err_bytes error = NO_MISTAKE;
    if(!intel){
        printf_to_log_file("NULL processor pointer\n");
        return INCORR_PROCESSOR_PTR;
    }
    if(!intel->code.comands){
        printf_to_log_file("NULL bytecode array pointer\n");
        return INCORR_BYTECODE_PTR;
    }
    if(intel->ic < 0 || intel->ic > intel->code.size){
        printf_to_log_file("Pointer to bytecode element out of index\n");
        printf_to_log_file("pointer %d\n", intel->ic);
        DEBUG(processor_dump(intel));
        error = error | INCORR_BYTECODE_ELEM_PTR;
    }
    error = error | stack_verify(intel->stack);
    return error;
}

void processor_free(processor* intel){
    if(!intel){
        fprintf(stderr, "Can't free data pointer to processor is NULL");
        return;
    }

    if(intel->stack){
        stack_free(intel->stack);
        intel->stack = NULL;
    }

    if(intel->code.comands){
        memset(intel->code.comands, 0, intel->code.size * sizeof(int));
        free(intel->code.comands);
        intel->code.comands = NULL;
    }

    if(intel->call_stack){
        stack_free(intel->call_stack);
        intel->call_stack = NULL;
    }
}

void fill_video_ram2(processor* intel){
    if(!intel){
        fprintf(stderr, "Pointer to processor is NULL - can't work");
        return;
    }
    int index = 0;
    double x_coord = 0;
    double y_coord = 0;
    int centr_x = WIDTH1 / 2;
    int centr_y = HEIGHT1 / 2;

    for(int y = 0; y < HEIGHT1; y++){
        for(int x = 0; x < WIDTH1; x++){
            index = (y * WIDTH1 + x);
            x_coord = fabs(x - centr_x);
            y_coord = fabs(y - centr_y);
            if(x_coord*x_coord + y_coord*y_coord <= R1*R1){
                intel->VIDEO_RAM2[index] = '*';
            }
            else{
                intel->VIDEO_RAM2[index] = ' ';
            }
        }
    }


    for(int y = 0; y < HEIGHT1; y++){
        for(int x = 0; x < WIDTH1; x++){
            index = (y * WIDTH1 + x);
            x_coord = fabs(x - centr_x);
            y_coord = fabs(y - centr_y);
            fprintf(stderr, "%c " ,intel->VIDEO_RAM2[index]);
        }
        fprintf(stderr, "\n");
    }
}








// Additional task - square r = 50
/*
void fill_video_ram(processor* intel){
    if(!intel){
        fprintf(stderr, "Pointer to processor is NULL - can't work");
        return;
    }
    int index = 0;
    double x_coord = 0;
    double y_coord = 0;
    int centr_x = WIDTH / 2;
    int centr_y = HEIGHT / 2;

    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            index = (y * WIDTH + x) * 3;
            x_coord = fabs(x - centr_x);
            y_coord = fabs(y - centr_y);
            if(x_coord*x_coord + y_coord*y_coord <= R){
                intel->VIDEO_RAM[index] = 199;
                intel->VIDEO_RAM[index + 1] = 21;
                intel->VIDEO_RAM[index + 2] = 133;
            }
            else{
                intel->VIDEO_RAM[index] = 0;
                intel->VIDEO_RAM[index + 1] = 0;
                intel->VIDEO_RAM[index + 2] = 0;
            }
        }
    }
}

void show_square(processor* intel){
    if(!intel){
        fprintf(stderr, "Pointer to processor is NULL - can't work");
        return;
    }

    sf::RenderWindow window(sf::VideoMode({WIDTH * PIXELSIZE, HEIGHT * PIXELSIZE}), "Display square");

    sf::RectangleShape pixel(sf::Vector2f(PIXELSIZE, PIXELSIZE));

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        
        if (window.isOpen()){
            for (int y = 0; y < HEIGHT; ++y){
                for (int x = 0; x < WIDTH; ++x){
                    int idx = (y * WIDTH + x) * 3;
                    pixel.setFillColor(sf::Color(
                        intel->VIDEO_RAM[idx], 
                        intel->VIDEO_RAM[idx + 1], 
                        intel->VIDEO_RAM[idx + 2]
                    ));
                    pixel.setPosition(sf::Vector2f(
                        x * PIXELSIZE, 
                        y * PIXELSIZE
                    ));
                    window.draw(pixel);
                }
            }
            
            window.display();
        }
    }
}
*/