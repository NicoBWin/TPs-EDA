/***************************************************************************//*
@file     +main.cpp+
@brief    +Archivo principal+
@author   +Grupo 1+
******************************************************************************/

/******************************************************************************
* INCLUDE HEADER FILES
******************************************************************************/
#include <iostream> 
#include <string>
#include <vector>

//DISPLAY CLASSES + INTERFACE
#include "../LCDs/basicLCD.h"
#include "../LCDs/NicoLCD/NicoLCD.h"
#include "../LCDs/FranLCD/DisplayFran.h"
#include "../LCDs/MiliLCD/milagrosLCD.h"

//IMGUI + ALLEGRO
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_allegro5.h"
#include "Allegro.h"

//FUNCIONES IMPORTANTES
#include "Client.h"     
#include "Printer.h"    //Incluye la librería NLOHMANN JSON.

using namespace std;

/******************************************************************************
* CONSTANT, ENUMS AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/
#define SPEED 0.1   //Seteamos la velocidad máxima (que implica el tiempo de refresco de pantalla mínimo)

/******************************************************************************
*******************************************************************************
*						            MAIN
*******************************************************************************
******************************************************************************/
int main(void) {
    if (init_allegro()) {           // inicio allegro y verifico que no haya errores
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplAllegro5_Init(displayAll);

        
        //Displays
        displayFran dFran; 
        DispN dNico;
        milagrosLCD dMili; 

        //Puntero a basicLCD (Interfaz)
        basicLCD* LCD = &dNico;

        nlohmann::json j;   //JSON donde se encontraran todos los tweets

        bool displayN = true;
        bool displayF = false;
        bool displayM = false;
        int tweetNumber = 0;    //Cantidad de Tweet a descargar
        int counter = 0;    //Numero de Tweet a visualizar (de los descargados) 

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);    //Color de fondo de la GUI

        bool running = true;
        bool changedDisplay = false;


        // ImGui Main loop
        while (running) {
            ALLEGRO_EVENT ev;

            //Cola de eventos de allegro
            while (al_get_next_event(event_queue, &ev)) {
                ImGui_ImplAllegro5_ProcessEvent(&ev);
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)     //Evento para cerrar el programa
                    running = false;

                if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                    ImGui_ImplAllegro5_InvalidateDeviceObjects();
                    al_acknowledge_resize(displayAll);
                    ImGui_ImplAllegro5_CreateDeviceObjects();
                }
                else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {  //apreto tecla?
                    switch (ev.keyboard.keycode) {             //que tecla apreto?
                    case ALLEGRO_KEY_UP:    //Cambiamos de tweet a mostrar
                        if (tweetNumber < counter - 1)
                            tweetNumber++;
                        break;
                    case ALLEGRO_KEY_DOWN:  //Cambiamos de tweet a mostrar
                        if (tweetNumber > 0)
                            tweetNumber--;
                        break;
                    }
                }
                else if (ev.type == ALLEGRO_EVENT_TIMER) {      //Refresco de pantalla
                    static int prevnum = -1;
                    static string msg;
                    static int prog;
                    static int msg_length;

                    if (LCD != nullptr) {
                        if (prevnum != tweetNumber || changedDisplay) {
                            if (prevnum != tweetNumber) {
                                msg = getTweet(j, tweetNumber);                 //Obtenemos el tweet en formato string
                                msg.insert(0, MAX_COL, ' ');                    //Agregamos espacios blancos para facilitar el scrolling
                                msg_length = msg.length();                      //Guardamos el tamaño del mensaje con los agregados
                                msg.insert(msg.length(), MAX_COL, ' ');         //Agregamos espacios blancos para facilitar el scrolling
                                prevnum = tweetNumber;
                            }

                            prog = 0;
                            LCD->lcdClear();
                            string date = getDate(j, tweetNumber);              //Obtenemos la fecha en formato string
                            *LCD << date.c_str();                               //Imprimimos la fecha en la fila 0
                            changedDisplay = false;
                        }

                        //Display Scroller
                        LCD->lcdSetCursorPosition({ MAX_ROW, 0 });           // nos paramos en el inicio de la 2da fila
                        *LCD << (msg.substr(prog, MAX_COL + 1)).c_str();     //printiamos el sgte caracter
                        prog++;                                              // incrementamos el cont de progreso
                        if (msg_length - prog < 0)                           //Si ya se termino de imprimir el MSG
                            prog = 0;                                        // reiniciamos el cont de progreso


                    }

                }
            }

            // Start the Dear ImGui frame
            ImGui_ImplAllegro5_NewFrame();
            ImGui::NewFrame();

            // Simple window for GUI
            {
                static int s = 30;

                ImGui::Begin("Tweets printer!");                          // Create a window.

                ImGui::Text("Tweets viewer 2.0");          // Display some text (you can use a format strings too)
                ImGui::Text("\n");
                ImGui::Text("Select 1 LCD to work with:");
                ImGui::Checkbox("Nico LCD", &displayN);      // Edit bools storing our window open/close displays
                ImGui::Checkbox("Fran LCD", &displayF);
                ImGui::Checkbox("Mili LCD", &displayM);
                ImGui::Text("\n");
                ImGui::Text("Show tweet number = %d", tweetNumber + 1);

                ImGui::SliderInt("Message scrolling speed [%]", &s, 1, 100);              // Edit speed by using int in slider from 0 to 100%
                al_set_timer_speed(timer, (SPEED * 100) / s);

                
                static char str[100]; 
                ImGui::InputTextWithHint("USER", "Ingrese el usuario", str, 100);
                string user(str);
                ImGui::Text("How many tweet do you want to download?");                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter = (counter < 1) ? 1 : counter;
                ImGui::InputInt("", &counter);  
                if (ImGui::Button("Download Tweets...")) {
                    if (!(client(j, counter, user))) {
                        std::cout << "Error occurred while downloading tweets" << endl;
                        std::cout << "Try again later" << endl;
                        running = false;
                    }
                    else {
                        al_start_timer(timer); // Comienza a correr el timer
                        changedDisplay = true;
                    }
                }

                ImGui::ColorEdit3("Select bkg. color", (float*)&clear_color);             // Edit 3 floats representing a color
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            //Conditions for checkboxs
            if (displayN) {
                if (LCD == nullptr) {
                    changedDisplay = true;
                }
                displayF = displayM = false;
                LCD = &dNico;    //Asignar el puntero LCD a display
            }
            else if (displayF) {
                if (LCD == nullptr) {
                    changedDisplay = true;
                }
                displayN = displayM = false;
                LCD = &dFran;     //Asignar el puntero LCD a display
            }
            else if (displayM) {
                if (LCD == nullptr) {
                    changedDisplay = true;
                }
                displayF = displayN = false;
                LCD = &dMili;     //Asignar el puntero LCD a display
            }
            else {
                LCD = nullptr;
            }

            // Rendering
            ImGui::Render();
            al_set_target_backbuffer(displayAll);
            al_clear_to_color(al_map_rgba_f(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w));
            ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
            al_flip_display();
        }
        // Cleanup
        ImGui_ImplAllegro5_Shutdown();
        ImGui::DestroyContext();
    }
    else {                      // error de inicializacion de allegro
        cout << endl;
        cout << "ALLEGRO ERROR" << endl;
        cout << endl;
    }
    destroy_allegro();      //Destruye recursos y finaliza la simulación
    cout << "See you next time..." << endl;
    return 0;
}