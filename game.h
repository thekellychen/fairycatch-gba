extern const unsigned short sinArray[256];

#define SIN(x) ((short)sinArray[(x)&0xFF])
#define COS(x) ((short)sinArray[((x)+0x40)&0xFF])

typedef struct {
  int posY;
  int posX;
} Tink;

typedef struct {
  int posY;
  int posX;
} Flower;

typedef struct {
  int posY;
  int posX;
} Beast;

void drawTink(Tink *aptr);
void drawBeast(Beast *bptr);
void drawFlower(Flower *fptr);
int collide(Flower *fptr, Beast *bptr);
