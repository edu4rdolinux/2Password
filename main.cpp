#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <cctype>
#include <vector>
#include <gtk/gtk.h>

std::string generate_password(int length, bool use_upper, bool use_lower, bool use_numbers, bool use_symbols) {
    std::string characters = "";
    if (use_upper) {
        characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (use_lower) {
        characters += "abcdefghijklmnopqrstuvwxyz";
    }
    if (use_numbers) {
        characters += "0123456789";
    }
    if (use_symbols) {
        characters += "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    }

    if (characters.empty()) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "At least one character type must be selected.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return "";
    }

    std::string password;
    std::default_random_engine generator(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<int> distribution(0, characters.size() - 1);

    for (int i = 0; i < length; ++i) {
        password += characters[distribution(generator)];
    }
    return password;
}

void on_generate(GtkWidget *widget, gpointer data) {
    GtkEntry *length_entry = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(data), "length_entry"));
    const gchar *length_text = gtk_entry_get_text(length_entry);
    int length = std::stoi(length_text);

    gboolean use_upper = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(GTK_BUILDER(data), "upper_check")));
    gboolean use_lower = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(GTK_BUILDER(data), "lower_check")));
    gboolean use_numbers = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(GTK_BUILDER(data), "number_check")));
    gboolean use_symbols = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(GTK_BUILDER(data), "symbol_check")));

    std::string password = generate_password(length, use_upper, use_lower, use_numbers, use_symbols);
    GtkLabel *result_label = GTK_LABEL(gtk_builder_get_object(GTK_BUILDER(data), "result_label"));
    gtk_label_set_text(result_label, password.c_str());
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "password_generator.glade", NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *generate_button = GTK_WIDGET(gtk_builder_get_object(builder, "generate_button"));
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate), builder);

    gtk_widget_show_all(window);
    gtk_main();

    g_object_unref(builder);
    return 0;
}