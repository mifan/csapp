#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
/* I don't know why */
/* #define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) ) */
#define BUF_LEN     ( 1024 * ( EVENT_SIZE ) )
/* #define MONIT_DIR "/home/mifan/source" */
#define MONIT_DIR "/home/mifan"




/* This method does the dirty work of determining what happened,
   then allows us to act appropriately
*/
void handle_event (struct inotify_event *event)
{
    /* If the event was associated with a filename, we will store it here */
    char * cur_event_filename = NULL;
    /* This is the watch descriptor the event occurred on */
    int cur_event_wd = event->wd;
    if (event->len)
    {
        cur_event_filename = event->name;
    }else{
        cur_event_filename = "***NOT A File***";
    }
    printf("*FILENAME=[%s]\n", cur_event_filename);

    /* Perform event dependent handler routines */
    /* The mask is the magic that tells us what file operation occurred */
    switch (event->mask)
    {
        /* File was accessed */
        case IN_ACCESS:
            printf("ACCESS EVENT OCCURRED: File \"%s\" on WD #%i\n",cur_event_filename, cur_event_wd);
            break;
        /* File was modified */
        case IN_MODIFY:
            printf("MODIFY EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File changed attributes */
        case IN_ATTRIB:
            printf("ATTRIB EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File was closed */
        case IN_CLOSE:
            printf("CLOSE EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File was opened */
        case IN_OPEN:
            printf("OPEN EVENT OCCURRED: File \"%s\" on WD #%i\n",cur_event_filename, cur_event_wd);
            break;
      /* File was moved from X */
        case IN_MOVED_FROM:
            printf("MOVE_FROM EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File was moved to X */
        case IN_MOVED_TO:
            printf("MOVE_TO EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File/Subdir was created */
        case IN_CREATE:
            printf("CREATE EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* File/Subdir was deleted */
        case IN_DELETE:
            printf("DELETE EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* Watched entry was deleted */
        case IN_DELETE_SELF:
            printf("DELETE_SELF EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* Self was moved. */
        case IN_MOVE_SELF:
            printf("MOVE_SELF EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* Backing FS was unmounted */
        case IN_UNMOUNT:
            printf("UNMOUNT EVENT OCCURRED: File \"%s\" on WD #%i\n", cur_event_filename, cur_event_wd);
            break;
      /* Too many FS events were received without reading them
         some event notifications were potentially lost.  */
        case IN_Q_OVERFLOW:
            printf("Warning: AN OVERFLOW EVENT OCCURRED: \n");
            break;
        case IN_IGNORED:
            printf("IGNORED EVENT OCCURRED: \n");
            break;
      /* Some unknown message received */
        default:
            printf ("UNKNOWN EVENT OCCURRED for file \"%s\" on WD #%i\n",cur_event_filename, cur_event_wd);
            break;
   }
}



int main( int argc, char **argv ) 
{
  int length, i = 0;
  int fd;
  int wd;
  char buffer[BUF_LEN];

  fd = inotify_init();

  if ( fd < 0 ) {
    perror( "inotify_init error . fd < 0" );
  }

  wd = inotify_add_watch( fd, MONIT_DIR, IN_MODIFY | IN_CREATE | IN_DELETE );
  printf("*** moniting dir :[%s]\n",MONIT_DIR);


/*
  length = read( fd, buffer, BUF_LEN );  
  
   
  if ( length < 0 ) {
    perror( "read error." );
  }
*/  
while(length = read( fd, buffer, BUF_LEN )) {
  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    handle_event(event);
    i += EVENT_SIZE + event->len;
  } /* inner while*/
  i = 0 ; /* reset i */
}
  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  exit( 0 );
}
