#include <game.h>
#include <klib.h>
int w,h,s,x,y;
int head,tail,my_w,my_h,line;
int decide,flag;
int circle,ttt;
void init_screen();
void splash();
void my_read_key();
void my_game();
void my_draw_rect();

int main() {
  // Operating system is a C program
  _ioe_init();
  init_screen();
  printf("%d %d\n",w,h);
  s=1;
  int cnt=0;
  while(1){
          my_game();
	  flag = 0;
	  cnt=0;
//	  puts("Please press r to restart the game or click the cross to shutdown the game!\n");
	  while(1){
		  while(uptime()<s*100) my_read_key();
		  if(flag) break;
		  cnt++;
		  s++;
		  if(cnt==20) {
                          puts("Please press R to restart the game or click the cross to shutdown the game!\n");
			  cnt=0;
		  }
		  
	  }
  }
/*  int sec=1;
  while(1){
  while(uptime() < sec*1000)
	  my_read_key();
  splash(sec & 1);
  sec++;
  my_read_key();
  }*/
  return 0;
}

void my_read_key() {
  _DEV_INPUT_KBD_t event = { .keycode = _KEY_NONE };
  #define KEYNAME(key) \
    [_KEY_##key] = #key,
  static const char *key_names[] = {
    _KEYS(KEYNAME)
  };
  _io_read(_DEV_INPUT, _DEVREG_INPUT_KBD, &event, sizeof(event));
  if (event.keycode != _KEY_NONE && event.keydown) {
//    puts("Key pressed: ");
//    puts(key_names[event.keycode]);
//    puts("\n");
          if(strcmp(key_names[event.keycode],"LEFT")==0){
	          if(head>=1) {
                          head-=1;
	                  my_draw_rect(head*5,line*5,5,5,0xffffff);
	                  my_draw_rect(tail*5,line*5,5,5,0x000000);
	                  tail-=1;
			  if(circle) {
				  x-=1;
				  my_draw_rect(x*5,y*5,5,5,0xffffff);
				  my_draw_rect((x+1)*5,y*5,5,5,0x000000);
			  }
	  	  }
	  }
	  else if(strcmp(key_names[event.keycode],"RIGHT")==0){
		  if(tail<=my_w-2){
			  tail+=1;
			  my_draw_rect(tail*5,line*5,5,5,0xffffff);
			  my_draw_rect(head*5,line*5,5,5,0x000000);
			  head+=1;
			  if(circle) {
				  x+=1;
				  my_draw_rect(x*5,y*5,5,5,0xffffff);
				  my_draw_rect((x-1)*5,y*5,5,5,0x000000);
			  }
		  }
	  }
	  else if(strcmp(key_names[event.keycode],"R")==0){
		  if(decide == 0)
			  flag=1;
	  }
	  else if(strcmp(key_names[event.keycode],"SPACE")==0){
		  if(circle){
			  ttt=1;
		  }
	  }

  }
}

//int w, h;

void init_screen() {
  _DEV_VIDEO_INFO_t info = {0};
  _io_read(_DEV_VIDEO, _DEVREG_VIDEO_INFO, &info, sizeof(info));
  w = info.width;
  h = info.height;
}

void my_draw_rect(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: allocated on stack
  _DEV_VIDEO_FBCTL_t event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  _io_write(_DEV_VIDEO, _DEVREG_VIDEO_FBCTL, &event, sizeof(event));
}

void splash(int d) {
  for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if ((x & 1) ^ (y & 1)) {
	if(d)
        my_draw_rect(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
	else
        my_draw_rect(x * SIDE, y * SIDE, SIDE, SIDE, 0x818181); 
      }
    }
  }
}

void my_game() {
	decide = 1;
	int i;
	my_h = h/5,my_w = w/5;
	x=my_w/2,y=my_h-3;
	head = x - 3;
	tail = x + 3;
	line = y + 2;
	for(i=head;i<=tail;i++) my_draw_rect(i*5,line*5,5,5,0xffffff);
	//int map[my_h][my_w];
	int dir[4][2]={{-1,-1},{1,-1},{1,1},{-1,1}};
	int my_dir=0;
	int ox,oy;
	my_draw_rect(x*5,y*5,5,5,0xffffff);
	circle=1;
	ttt=0;
	while(1){
		while(uptime()<100*s) my_read_key();
		if(ttt) break;
		s++;
		if(s%20==0)
			puts("Use RIGHT and LEFT to locate your start position and press SPACE when you are ready!!!\n");
	}
	circle=0;
	//memset(map,0,sizeof(map));
	while(1){
		while(uptime()<100*s) my_read_key();
		s++;
//		printf("%d %d\n",x,y);
		if(x>=1&&x<=my_w-2&&y>=1&&y<=my_h-3){
			ox=x,oy=y;
			x+=dir[my_dir][0];
			y+=dir[my_dir][1];
//			printf("%d\n",my_dir);
			my_draw_rect(ox*5,oy*5,5,5,0x000000);
			my_draw_rect(x*5,y*5,5,5,0xffffff);
		}
		else{
			if(y==my_h-2&&(head>x||tail<x)){
				printf("Your ball has fallen on the ground!\n");
				my_draw_rect(x*5,y*5,5,5,0x000000);
				for(i=head;i<=tail;i++) my_draw_rect(i*5,line*5,5,5,0x000000);
				break;
			}
			switch(my_dir){
				case 0:
					if(x==0&&y!=0){
						my_dir = 1;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					else{
                                                my_dir = 3;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					break;
				case 1:
					if(y==0&&x!=my_w-1){
                                                my_dir = 2;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					else{
                                                my_dir = 0;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					break;
				case 2:
					if(x==my_w-1&&y!=my_h-2){
                                                my_dir = 3;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					else{
                                                my_dir = 1;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					break;
				case 3:
					if(y==my_h-2&&x!=0){
                                                my_dir = 0;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					else{
                                                my_dir = 2;
						ox=x;oy=y;
						x+=dir[my_dir][0];
						y+=dir[my_dir][1];
						my_draw_rect(ox*5,oy*5,5,5,0x000000);
						my_draw_rect(x*5,y*5,5,5,0xffffff);
					}
					break;
			}
		}
	}
	decide = 0;
}













	


