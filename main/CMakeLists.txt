idf_component_register(SRCS "demo.c"

                            "src/chaosi_system_data.c"
                            
                            "src/fileSystem/MyfileSystem.c"

                            "src/hal/chaosi_nvs.c"
                            "src/hal/chaosi_encoder.c"
                            "src/hal/rotary_encoder_pcnt_ec11.c"
                            "src/hal/chaosi_timer.c"
                         

                            "src/anime/chaosi_anime.c"


                            "src/net/chaosi_wifi_ap.c"
                            "src/net/chaosi_wifi_sta.c"
                            "src/net/chaosi_wifi_ap_sta.c"
                            "src/net/chaosi_http_server.c"
                            "src/net/chaosi_http_request.c"
                            


                            "src/gui_demo/bg_screen.c"
                            "src/gui_demo/menu_template.c"
                            "src/gui_demo/home_page.c"
                            "src/gui_demo/time_page.c"
                            "src/gui_demo/wifi_page.c"
                            "src/gui_demo/bilibili_page.c"
                            "src/gui_demo/flush_page.c"
                            "src/gui_demo/lv_disp.c"

                            "src/app/time_app.c"
                            "src/app/wifi_app.c"
                         

                        INCLUDE_DIRS
                            
                             "./include"
                             "./include/fileSystem"
                             "./include/hal"
                             "./include/net"
                             "./include/gui_demo"
                             "./include/anime"
                             "./include/app"
                             "../components/user_dev"
                        EMBED_FILES
                             "setting.html"
                             "upload_script.html"
                             "favicon.ico"
                             
                             
                             
                             )
