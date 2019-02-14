/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:27:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/02/14 18:00:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"
#include <math.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#define USAGE "licorne"

const float WIDTH = 1000, HEIGHT = 1000, HEIGHT_DIV = HEIGHT / 2.0, WIDTH_DIV = WIDTH / 2.0;

static int close_main(int status, char *str)
{
	ft_putendl(str);
	return (status);
}

#define mapWidth 24
#define mapHeight 24

int worldMap[mapHeight][mapWidth] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct	s_cam
{
	float rotation;
	float plane;
	t_vec2 pos;
	t_vec2 dir;
	t_mat2 matrix;

} t_cam;

double ticks_to_ms(clock_t ticks)
{
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

int	main(int argc, char **argv)
{
	t_inst inst;

	if (argc != 2)
		return (close_main(1, USAGE));
	(void)argv;
	(void)inst;
	//read_map(&inst, argv[1]);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
	SDL_RenderSetLogicalSize(renderer, WIDTH_DIV, HEIGHT_DIV);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	//	float t = 0;
	t_cam camera = (t_cam) {.pos = (t_vec2) {1, 1}, .plane = WIDTH_DIV / WIDTH,
							.dir = (t_vec2) {0, 1}};
	SDL_Surface *image = IMG_Load("textures/walls.png");
	//SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
	TTF_Font *font = TTF_OpenFont("assets/fonts/helvetica.ttf", 22);
	SDL_Surface *screen =  SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0,0,0,0);
	//SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

	clock_t delta = 0;
	unsigned int frames = 0;
	double fps = 0;
	double avg_time = 0;
	while (1)
	{

		clock_t t1, t2;
		t1 = clock();
		//	t += 0.0006;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
	//	SDL_SetRenderTarget(renderer, texture);
		int i = 0;
		float camX = 2.0f * (float)i / WIDTH - 1.0f;
		t_vec2 r = ft_mat2_mulv(camera.matrix, ft_vec2_add(camera.dir,
			(t_vec2) {camera.plane * camX, 0}));
		camera.matrix = ft_mat2_rotation(camera.rotation - M_PI_2);
		//	camera.rotation = t;
		SDL_memset(screen->pixels, 0, screen->h * screen->pitch);
		for (i = 0; i < WIDTH; i++)
		{
			camX = 2.0f * (float)i / WIDTH - 1.0f;
			r = ft_mat2_mulv(camera.matrix, ft_vec2_add(camera.dir,
				(t_vec2) {camera.plane * camX, 0}));
			t_pixel map = (t_pixel) {.x = camera.pos.x, .y = camera.pos.y};
			t_vec2 side_dist = (t_vec2) {0, 0};
			t_vec2 delta_dist = (t_vec2) {ft_absf(1.0f / r.x), ft_absf(1.0f /
				r.y)};
			float dist = 0;
			t_pixel step;
			int hit = 0;
			int side;
			if (r.x < 0)
			{
				step.x = -1;
				side_dist.x = (camera.pos.x - (float)map.x) * delta_dist.x;
			}
			else
			{
				step.x = 1;
				side_dist.x = ((float)map.x + 1.0 - camera.pos.x) *
					delta_dist.x;
			}
			if (r.y < 0)
			{
				step.y = -1;
				side_dist.y = (camera.pos.y - (float)map.y) * delta_dist.y;
			}
			else
			{
				step.y = 1;
				side_dist.y = ((float)map.y + 1.0 - camera.pos.y) *
					delta_dist.y;
			}
			while (hit == 0)
			{
				//jump to next map square, OR in x-direction, OR in y-direction
				if (side_dist.x < side_dist.y)
				{
					side_dist.x += delta_dist.x;
					map.x += step.x;
					side = 0;
				}
				else
				{
					side_dist.y += delta_dist.y;
					map.y += step.y;
					side = 1;
				}
				hit = worldMap[map.x][map.y];
			}
			if (side == 0)
				dist = (map.x - camera.pos.x + (1 - step.x) / 2.0) / r.x;
			else
				dist = (map.y - camera.pos.y + (1 - step.y) / 2.0) / r.y;
			
			if (side == 0 && r.x < 0)	//NORTH
				hit = 3;
			if (side == 0 && r.x > 0)	//SOUTH
				hit = 5;
			if (side == 1 && r.y > 0)	//OUEST
				hit = 7;
			if (side == 1 && r.y < 0)	//EST
				hit = 9;

			double wallX;	//where exactly the wall was hit
			if (side == 0)
				wallX = camera.pos.y + dist * r.y;
			else
				wallX = camera.pos.x + dist * r.x;
			wallX -= floor((wallX));
			int texWidth = 64;
			int texX = wallX * texWidth;
			if (side == 0 && r.x > 0)
				texX = texWidth - texX - 1;
			if (side == 1 && r.y < 0)
				texX = texWidth - texX - 1;
			if (dist < 0.0001)
				dist = 0.0001;
			int height = HEIGHT / dist / 2.0;
			t_color color = (t_color) {255, 255, 255, 0};
			if (hit == 0)
				color = (t_color) {255, 0, 0, 0};
			if (hit == 2)
				color = (t_color) {0, 255, 0, 0};
			if (hit == 4)
				color = (t_color) {0, 0, 255, 0};
			if (hit == 6)
				color = (t_color) {255, 0, 255, 0};
			color = ft_color_gradient(color, (t_color) {color.r + 20, color.g + 20, color.b + 20, 0}, dist / 24);
			/*SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_RenderDrawLine(renderer, i, HEIGHT_DIV - height, i, HEIGHT_DIV + height);*/

			SDL_Rect dst = {i, HEIGHT_DIV - height, 1, height * 2 + 1};
			SDL_Rect src = {texX + ((hit + (side == 1)) % 6) * 64, ((hit + (side == 1)) / 6) * 64, 1, 64};
			SDL_BlitScaled(image, &src, screen, &dst);

			t_vec2 floorWall;
			if (side == 0 && r.x > 0)
				floorWall = (t_vec2) {map.x, map.y + wallX};
			else if (side == 0 && r.x < 0)
				floorWall = (t_vec2) {map.x + 1.0, map.y + wallX};
			else if (side == 1 && r.y > 0)
				floorWall = (t_vec2) {map.x + wallX, map.y};
			else
				floorWall = (t_vec2) {map.x + wallX, map.y + 1.0};
			double distWall, distPlayer, currentDist;
			int drawEnd = (HEIGHT_DIV + height);
			distWall = dist;
			distPlayer = 0.0;
			for (int y = drawEnd + 1; y < HEIGHT; y++)
			{
				currentDist = HEIGHT / (2 * y - HEIGHT);
				double weight = (currentDist - distPlayer) / (distWall - distPlayer);
				t_vec2 curr_floor = (t_vec2)
				{
					weight * floorWall.x + (1.0 - weight) * camera.pos.x,
					weight * floorWall.y + (1.0 - weight) * camera.pos.y
				};
				int floorTexX, floorTexY;
				floorTexX = (int)(curr_floor.x * texWidth) % texWidth;
				floorTexY = (int)(curr_floor.y * 64) % 64;
				SDL_Rect dstrect = {i, y, 1, 1};
				SDL_Rect srcrect = {floorTexX, floorTexY, 1, 1};
				SDL_BlitSurface(image, &srcrect, screen, &dstrect);
				dstrect = (SDL_Rect){i, HEIGHT - y, 1, 1};
				srcrect = (SDL_Rect){floorTexX + 64 * 5, floorTexY + 64 * 3, 1, 1};
				SDL_BlitSurface(image, &srcrect, screen, &dstrect);
				//SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
				/*	SDL_SetRenderDrawColor(renderer, 255 / currentDist, 255,
				   255, 255);
				    SDL_RenderDrawPoint(renderer, i, y);
				    SDL_RenderDrawPoint(renderer, i, HEIGHT - y);*/
			}
		}
		
		if (event.type == SDL_QUIT || (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
			break;
		if (event.type == SDL_KEYDOWN && (SDL_SCANCODE_W == event.key.keysym.scancode || SDL_SCANCODE_S == event.key.keysym.scancode))
		{
			camera.pos.y += sinf(camera.rotation) * (SDL_SCANCODE_W ==  event.key.keysym.scancode ? 1 : -1) * 0.5;
			camera.pos.x += cosf(camera.rotation) * (SDL_SCANCODE_W == event.key.keysym.scancode ?  1 : -1) * 0.5;
		}
		if (event.type == SDL_KEYDOWN && (SDL_SCANCODE_A == event.key.keysym.scancode || SDL_SCANCODE_D == event.key.keysym.scancode))
		{
			camera.pos.y += -cosf(camera.rotation) * (SDL_SCANCODE_D == event.key.keysym.scancode ? 1 : -1) * 0.5;
			camera.pos.x += sinf(camera.rotation) * (SDL_SCANCODE_D ==  event.key.keysym.scancode ? 1 : -1) * 0.5;
		}
		if (event.type == SDL_KEYDOWN && (SDL_SCANCODE_J == event.key.keysym.scancode || SDL_SCANCODE_L == event.key.keysym.scancode))
			camera.rotation += 0.05 * (SDL_SCANCODE_J == event.key.keysym.scancode ? 1 : -1);
		/*if (event.type == SDL_KEYDOWN)
		    printf("pos : %f %f , rot : %f\n", r.x, r.y, camera.rotation);*/
		t2 = clock();
		delta += t2 - t1;
		frames++;
		if (ticks_to_ms(delta) > 1000.0)
		{
			fps = (double)frames * 0.5 + fps * 0.5;
			frames = 0;
			delta -= CLOCKS_PER_SEC;
			avg_time = 1000 / (fps == 0 ? 0.001 : fps);
		}
		SDL_Color color = {255, 255, 255};
		SDL_Surface *text = TTF_RenderText_Blended(font, ft_itoa(fps), color);
		SDL_Rect dest = {5, 5, 25, 25};
		SDL_BlitSurface(text, NULL, screen, &dest);
		
		SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, screen);
		SDL_RenderCopy(renderer, texture1, NULL, NULL);
	
		//SDL_UnlockTexture(texture);
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(texture1);
		SDL_PollEvent(&event);
	}
	//SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return (0);
}
