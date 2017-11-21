#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <gtk/gtk.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

GtkBuilder *builder; 
GtkWidget *window;
GtkWidget	*entry_word;
GtkTextBuffer *text_mean;

int sockfd;
struct sockaddr_in servaddr;
char sendline[MAXLINE], recvline[MAXLINE];
void on_entry_word_activate(){
  const gchar *word = gtk_entry_get_text(GTK_ENTRY(entry_word));
  strcpy(sendline,"\0");
  strcpy(sendline, word);
  gtk_entry_set_text(GTK_ENTRY(entry_word),"");
  send(sockfd, sendline, strlen(sendline)+1, 0);
		
  if (recv(sockfd, recvline, MAXLINE,0) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely"); 
   exit(4);
  }
  char intro[MAXLINE];
  strcpy(intro, "String received from the server: ");
  strcat(intro,recvline);
  gtk_text_buffer_set_text(GTK_TEXT_BUFFER(text_mean), intro, -1);
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

int
main(int argc, char **argv) 
{	
 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server"); 
  exit(1);
 }
	
 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
	
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
	
 gtk_init(&argc, &argv);
 builder = gtk_builder_new();
 gtk_builder_add_from_file (builder, "Window.glade", NULL);
 window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
 gtk_builder_connect_signals(builder, NULL);
 

 entry_word = GTK_WIDGET(gtk_builder_get_object(builder, "entry_word"));
 text_mean = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "text_mean"));

 g_object_unref(builder);
 
 gtk_widget_show(window);             
 gtk_main();
 

 return 0;	

 exit(0);
}

