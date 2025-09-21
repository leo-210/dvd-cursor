#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <suinput.h>

#define WIDTH 1920
#define HEIGHT 1080
// Pixels per 50 ms
#define SPEED 5
// In secs
#define COOLDOWN 5
#define DIAG_THRESHOLD 5
#define COLLISION_THREASHOLD 2

void move_mouse(int fd, int dx, int dy, int *curr_x, int *curr_y) {
	suinput_emit(fd, EV_REL, REL_X, dx);
	suinput_emit(fd, EV_REL, REL_Y, dy);
	suinput_syn(fd);

	*curr_x += dx;
	*curr_y += dy;
}

int main(void) {
	int uinput_fd;
	int rel_axes[] = {REL_X, REL_Y, REL_WHEEL};
	struct uinput_user_dev user_dev;
	int i;

	memset(&user_dev, 0, sizeof(struct uinput_user_dev));
	strcpy(user_dev.name, "dvd-cursor-mouse");

	uinput_fd = suinput_open();

	if (uinput_fd == -1) {
		printf("error: cannot create virtual device\n");
		return EXIT_FAILURE;
	}

	for (i = 0; i < 3; ++i) {
		suinput_enable_event(uinput_fd, EV_REL, rel_axes[i]);
	}

	suinput_create(uinput_fd, &user_dev);
	sleep(1);

	int events_fd = open("/dev/input/mice", O_RDONLY | O_NONBLOCK);
	if (events_fd == -1) {
		printf("error: cannot read /dev/input/mice\n");
		return EXIT_FAILURE;
	}

	srand((unsigned)time(NULL));
	int curr_x;
	int curr_y;
	int dx = SPEED;
	int dy = SPEED;

	signed char buf[6];

	bool is_afk = false;
	clock_t not_afk_since = clock();

	while (1) {
		if (is_afk) {
			usleep(50 * 1000);
			move_mouse(uinput_fd, dx, dy, &curr_x, &curr_y);

			if (curr_x <= COLLISION_THREASHOLD)
				dx = SPEED;
			else if (curr_x >= WIDTH - COLLISION_THREASHOLD)
				dx = -SPEED;
			else if (curr_y <= COLLISION_THREASHOLD)
				dy = SPEED;
			else if (curr_y >= HEIGHT - COLLISION_THREASHOLD)
				dy = -SPEED;
		} else {
			float afk_for_secs =
				(float)(clock() - not_afk_since) / CLOCKS_PER_SEC;
			if (afk_for_secs > 5) {
				is_afk = true;
				printf("user is now afk\n");

				curr_x = (int)(rand() % (WIDTH / 2) + WIDTH / 4);
				curr_y = (int)(rand() % (HEIGHT / 2) + HEIGHT / 4);

				// align cursor to random width and height
				for (int i = 0; i < WIDTH / 4; i++) {
					suinput_emit(uinput_fd, EV_REL, REL_X, -5);
					suinput_syn(uinput_fd);
					usleep(1 * 1000);
				}
				for (int i = 0; i < HEIGHT / 4; i++) {
					suinput_emit(uinput_fd, EV_REL, REL_Y, -5);
					suinput_syn(uinput_fd);
					usleep(1 * 1000);
				}
				usleep(50 * 1000);

				for (int i = 0; i < curr_x; i++) {
					suinput_emit(uinput_fd, EV_REL, REL_X, 1);
					suinput_syn(uinput_fd);
					usleep(5 * 1000);
				}
				for (int i = 0; i < curr_y; i++) {
					suinput_emit(uinput_fd, EV_REL, REL_Y, 1);
					suinput_syn(uinput_fd);
					usleep(5 * 1000);
				}
				usleep(50 * 1000);
			}
		}

		if (read(events_fd, buf, sizeof(buf)) > 0) {
			not_afk_since = clock();
			if (is_afk) {
				printf("user is back\n");
			}
			is_afk = false;
		}
	}

	suinput_destroy(uinput_fd);
	close(events_fd);

	return EXIT_SUCCESS;
}
