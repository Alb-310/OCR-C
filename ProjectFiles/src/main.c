#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include ".h/imageprocessing.h"
#include ".h/extraction.h"

#include ".h/extractMethods.h"
#include ".h/matrice.h"

#include ".h/nn_tools.h"

typedef struct {
    GtkWidget *w_dlg_file_choose;       // Pointer to file chooser dialog box
    GtkWidget *w_img_main;              // Pointer to image widget
    GtkWidget *w_txt_main;              // Pointer to text widget
    GtkWidget *w_btn_left;              // Pointer to rotate left button widget
    GtkWidget *w_btn_right;              // Pointer to rotate right button widget
    GtkWidget *w_btn_run;              // Pointer to run button widget
    GtkWidget *w_btn_save;              // Pointer to save button widget
} app_widgets;

gchar* fileName;

int fileCount(char* path)
{
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(path); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == 8) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);

    return file_count;
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_file_choose"));
    widgets->w_img_main = GTK_WIDGET(gtk_builder_get_object(builder, "img_main"));
    widgets->w_txt_main = GTK_WIDGET(gtk_builder_get_object(builder, "txt_main"));
    widgets->w_btn_left = GTK_WIDGET(gtk_builder_get_object(builder, "btn_rotate_left"));
    widgets->w_btn_right = GTK_WIDGET(gtk_builder_get_object(builder, "btn_rotate_right"));
    widgets->w_btn_run = GTK_WIDGET(gtk_builder_get_object(builder, "btn_run"));
    widgets->w_btn_save = GTK_WIDGET(gtk_builder_get_object(builder, "btn_save"));

    // Convert all widgets
    GtkTextView *text_view = GTK_TEXT_VIEW(widgets->w_txt_main);

    // Diable all unnecessary widgets
    gtk_text_view_set_editable (text_view, FALSE);
    gtk_text_view_set_cursor_visible(text_view, FALSE);
    gtk_widget_set_sensitive (widgets->w_btn_left, FALSE);
    gtk_widget_set_sensitive (widgets->w_btn_right, FALSE);
    gtk_widget_set_sensitive (widgets->w_btn_run, FALSE);
    gtk_widget_set_sensitive (widgets->w_btn_save, FALSE);
    
    // Build all signal
    gtk_builder_connect_signals(builder, widgets);

    // Set default background
    GdkPixbuf *img_displayed = gdk_pixbuf_new_from_file("config/background.png", NULL);
    int H = gdk_pixbuf_get_height (img_displayed);
    int W = gdk_pixbuf_get_width (img_displayed);

    gtk_image_set_from_pixbuf(GTK_IMAGE(widgets->w_img_main), gdk_pixbuf_scale_simple(img_displayed, W, H, GDK_INTERP_BILINEAR));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

// File --> Open
void on_btn_open_clicked(GtkButton *button, app_widgets *app_wdgts)
{   
    // Show the "Open Image" dialog box
    gtk_widget_show(app_wdgts->w_dlg_file_choose);
    
    // Check return value from Open Image dialog box to see if user clicked the Open button
    if (gtk_dialog_run(GTK_DIALOG (app_wdgts->w_dlg_file_choose)) == GTK_RESPONSE_OK) {

        // Get the file name from the dialog box and create a GdkPixbuf data
        gchar *file_name = gtk_file_chooser_get_filename(
                GTK_FILE_CHOOSER(app_wdgts->w_dlg_file_choose));
        
        // Scale and set the content of Gtk Image Widget
        if (file_name != NULL) {

            system("sudo rm -r -f cache/*");
            GtkTextView *text_view = GTK_TEXT_VIEW(app_wdgts->w_txt_main);
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
            gtk_text_buffer_set_text(buffer, "", 0);

            SDL_Surface *image = IMG_Load(file_name);
            SDL_SaveBMP(image, "cache/init.bmp");
            GdkPixbuf *img_displayed = gdk_pixbuf_new_from_file("cache/init.bmp", NULL);           
            int H = gdk_pixbuf_get_height (img_displayed);
            int W = gdk_pixbuf_get_width (img_displayed);
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_wdgts->w_img_main), gdk_pixbuf_scale_simple(img_displayed, W, H, GDK_INTERP_BILINEAR));

            gtk_widget_set_sensitive (app_wdgts->w_btn_left, TRUE);
            gtk_widget_set_sensitive (app_wdgts->w_btn_right, TRUE);
            gtk_widget_set_sensitive (app_wdgts->w_btn_run, TRUE);

        }

        printf("%ld\n", sizeof(button));
    }

    // Finished with the "Open Image" dialog box, so hide it
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}

// File --> Quit
void on_btn_exit_clicked(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    printf("%ld\n", sizeof(menuitem));
    printf("%ld\n", sizeof(app_wdgts));
    system("sudo rm -r -f cache/*");
    gtk_main_quit();
}

void on_btn_rotate_left_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    printf("%ld\n", sizeof(button));
    image_rotation("cache/init.bmp", 1);
    
    GdkPixbuf *img_displayed = gdk_pixbuf_new_from_file("cache/init.bmp", NULL);
    int H = gdk_pixbuf_get_height (img_displayed);
    int W = gdk_pixbuf_get_width (img_displayed);

    gtk_image_set_from_pixbuf(GTK_IMAGE(app_wdgts->w_img_main), gdk_pixbuf_scale_simple(img_displayed, W, H, GDK_INTERP_BILINEAR));
}

void on_btn_rotate_right_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    printf("%ld\n", sizeof(button));
    image_rotation("cache/init.bmp", -1);
    
    GdkPixbuf *img_displayed = gdk_pixbuf_new_from_file("cache/init.bmp", NULL);
    int H = gdk_pixbuf_get_height (img_displayed);
    int W = gdk_pixbuf_get_width (img_displayed);

    gtk_image_set_from_pixbuf(GTK_IMAGE(app_wdgts->w_img_main), gdk_pixbuf_scale_simple(img_displayed, W, H, GDK_INTERP_BILINEAR));
}

void on_btn_save_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    printf("%ld\n", sizeof(button));
    GtkTextView *text_view = GTK_TEXT_VIEW(app_wdgts->w_txt_main);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

    GtkTextIter start, end;
    gchar *text;

    gtk_text_buffer_get_bounds (buffer, &start, &end);
    text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

    FILE* fichier = NULL;
 
    fichier = fopen("cache/output.txt", "w");
 
    if (fichier != NULL)
    {
        fputs(text, fichier);
        fclose(fichier);
    } 

    g_free(text);
}

// called when window is closed
void on_window_main_destroy()
{
    system("sudo rm -r -f cache/*");
    gtk_main_quit();
}

// run the OCR
void on_btn_run_clicked(GtkButton *button, app_widgets *app_wdgts)
{
    printf("[\033[31mBEGIN\033[0m] - Run OCR\n----------\n");
    // Convert all widgets
    GtkTextView *text_view = GTK_TEXT_VIEW(app_wdgts->w_txt_main);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    char* arrayG[1];

    printf("[\033[31mBEGIN\033[0m] - ImageProcessing\n");

    // apply 4 filters
    for (int i = 1; i <= 4; i++)
    {
        if (i == 1)
        {
            arrayG[0] = "cache/init.bmp";
            Image_Processing(arrayG, i);
        }

        else
        {     
            arrayG[0] = "cache/result.bmp";
            Image_Processing(arrayG, i);
        }   
    }

    printf("[\033[32mEND\033[0m] - ImageProcessing\n");

    // Create new folder
    mkdir("cache/blocs", 0777);
    mkdir("cache/words", 0777);
    mkdir("cache/characters", 0777);
    mkdir("cache/lines", 0777);

    // extraction of blocs
    arrayG[0] = "cache/result.bmp";
    extractB(arrayG);

    // count number of files
    int file_count = fileCount("cache/blocs");

    printf("[\033[31mBEGIN\033[0m] - ExtractData\n");
    // initialize the Network
    struct NN *net = ExtractData();
    printf("[\033[32mEND\033[0m] - ExtractData\n");

    printf("[\033[31mBEGIN\033[0m] - Segmentation & Neural Network\n");
    // extraction
    for(int i = 0; i < file_count; i++)
    {       
        char path[] = "cache/blocs/extract_B";
        char number[500];
        char ext[] = ".bmp";

        sprintf(number, "%d", i);

        strcat(path, number);  
        strcat(path, ext);

        arrayG[0] = path;
        extractL(arrayG);

        // count number of files
        int file_count = fileCount("cache/lines");
        for(int j = 0; j < file_count; j++)
        {        
            char path[] = "cache/lines/extract_L";
            char number[500];
            char ext[] = ".bmp";

            sprintf(number, "%d", j);

            strcat(path, number);  
            strcat(path, ext);

            arrayG[0] = path;
            extractW(arrayG);

            // count number of files
            int file_count = fileCount("cache/words");

            for(int k = 0; k < file_count; k++)
            {        
                char path[] = "cache/words/extract_W";
                char number[500];
                char ext[] = ".bmp";

                sprintf(number, "%d", k);

                strcat(path, number);  
                strcat(path, ext);

                arrayG[0] = path;
                extractC(arrayG, buffer, net);
                gtk_text_buffer_insert_at_cursor (buffer, " ", 1);
            } 
            gtk_text_buffer_insert_at_cursor (buffer, "\n", 1); 
        }
        system("rm cache/lines/* cache/words/* cache/characters/*");
        gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);    
    }

    printf("[\033[32mEND\033[0m] - Segmentation & Neural Network\n----------\n");

    // Enable all necessary widgets
    gtk_text_view_set_editable (text_view, TRUE);
    gtk_text_view_set_cursor_visible(text_view, TRUE);
    gtk_widget_set_sensitive(app_wdgts->w_btn_save, TRUE);

    // disable all unnecessary widgets
    
    gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    gtk_widget_set_sensitive (app_wdgts->w_btn_left, FALSE);
    gtk_widget_set_sensitive (app_wdgts->w_btn_right, FALSE);

    printf("[\033[32mEND\033[0m] - Run OCR\n");
}