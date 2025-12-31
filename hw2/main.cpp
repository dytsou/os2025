#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/eventfd.h>

int main()
{
  // Variable declarations - grouped by type
  int color_index = 0;
  bool running = true;
  int screen_height, screen_width, user_input, current_y, current_x, new_y, new_x;
  time_t current_time;
  char time_string[64];
  struct pollfd input_poll_fds[1];

  // Initialize poll structure
  input_poll_fds[0].fd = STDIN_FILENO;
  input_poll_fds[0].events = POLLIN;

  // Initialize ncurses
  initscr();
  raw();                    // Enter raw mode
  keypad(stdscr, TRUE);     // Enable special keystrokes
  noecho();                 // Hide typed characters
  curs_set(0);              // Hide cursor
  start_color();

  // Define color pairs
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);

  // Initialize position variables
  new_x = new_y = current_x = current_y = 0;

  // Main application loop
  while (running)
  {
    clear();  // Clear screen

    // Get and format current time
    current_time = time(0);
    strftime(time_string, sizeof(time_string), "%c", localtime(&current_time));
    int string_length = strlen(time_string);

    // Get screen dimensions and validate position
    getmaxyx(stdscr, screen_height, screen_width);
    if (new_y < 0) new_y = 0;
    if (new_y >= screen_height) new_y = screen_height - 1;
    if (new_x + string_length >= screen_width) new_x = screen_width - string_length;
    if (new_x < 0) new_x = 0;

    // Display time with current color
    attron(COLOR_PAIR(color_index + 1));
    wmove(stdscr, new_y, new_x);
    waddstr(stdscr, time_string);
    wrefresh(stdscr);
    attroff(COLOR_PAIR(color_index + 1));

    // Advance to next color
    color_index = (color_index + 1) % 7;

    // Update position tracking
    current_y = new_y;
    current_x = new_x;

    // Handle user input with 1 second timeout
    if (poll(input_poll_fds, 1, 1000) > 0 && (input_poll_fds[0].revents & POLLIN))
    {
      user_input = getch();
      switch (user_input)
      {
        case KEY_UP:
          new_y = current_y - 1;
          break;
        case KEY_DOWN:
          new_y = current_y + 1;
          break;
        case KEY_LEFT:
          new_x = current_x - 1;
          break;
        case KEY_RIGHT:
          new_x = current_x + 1;
          break;
        case 'q':
        case 'Q':
          running = false;
          break;
        default:
          break;
      }
    }
  }

  // Cleanup and exit
  wrefresh(stdscr);
  endwin();
  return 0;
}