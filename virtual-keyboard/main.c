#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/uinput.h>

/*
 * Descrição:
 *  Função para emitir o evento de click em uma tecla. 
 * 
 * Parametros:
 *  fd: Descritor de arquivo
 *  type: Tipo de ação.
 *  code: Código da tecla.
 *  value: Ação a ser executada.
 * 
 * Exemplo:
 *  Precionar a tecla espaço. 
 * 
 *  emit(fd, EV_KEY, KEY_SPACE, 1);
 *  emit(fd, EV_SYN, SYN_REPORT, 0);
 *  emit(fd, EV_KEY, KEY_SPACE, 0);
 *  emit(fd, EV_SYN, SYN_REPORT, 0);  
 */
void emit(int fd, int type, int code, int value) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = value;

  write(fd, &ie, sizeof(ie));
}

int keyCode(char key) {
  switch (key) {
    case '0': return KEY_SPACE;
    case '1': return KEY_ENTER;
    case '2': return KEY_BACKSPACE;
    case 'A': return KEY_A;
    case 'B': return KEY_B;
    case 'C': return KEY_C;
    case 'D': return KEY_D;
    case 'E': return KEY_E;
    case 'F': return KEY_F;
    case 'G': return KEY_G;
    case 'H': return KEY_H;
    case 'I': return KEY_I;
    case 'J': return KEY_J;
    case 'K': return KEY_K;
    case 'L': return KEY_L;
    case 'M': return KEY_M;
    case 'N': return KEY_N;
    case 'O': return KEY_O;
    case 'P': return KEY_P;
    case 'Q': return KEY_Q;
    case 'R': return KEY_R;
    case 'S': return KEY_S;
    case 'T': return KEY_T;
    case 'U': return KEY_U;
    case 'V': return KEY_V;
    case 'W': return KEY_W;
    case 'X': return KEY_X;
    case 'Y': return KEY_Y;
    case 'Z': return KEY_Z;
    default:
      break;
  }
}

/* Preciona a tecla, reporta o evento, libera a tecla e reporta novamente */
void click(int fd, char key) {
  emit(fd, EV_KEY, keyCode(key), 1);
  emit(fd, EV_SYN, SYN_REPORT, 0);
  emit(fd, EV_KEY, keyCode(key), 0);
  emit(fd, EV_SYN, SYN_REPORT, 0);
}

int main(int argc, char* argv[]) {
  struct uinput_setup usetup;
  
  // Abrindo o descritor de arquivo
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  // Definindo a teclas que nós queremos simular.
  ioctl(fd, UI_SET_EVBIT, EV_KEY);
  ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);
  ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
  ioctl(fd, UI_SET_KEYBIT, KEY_BACKSPACE);
  ioctl(fd, UI_SET_KEYBIT, KEY_A);
  ioctl(fd, UI_SET_KEYBIT, KEY_B);
  ioctl(fd, UI_SET_KEYBIT, KEY_C);
  ioctl(fd, UI_SET_KEYBIT, KEY_D);
  ioctl(fd, UI_SET_KEYBIT, KEY_E);
  ioctl(fd, UI_SET_KEYBIT, KEY_F);
  ioctl(fd, UI_SET_KEYBIT, KEY_G);
  ioctl(fd, UI_SET_KEYBIT, KEY_H);
  ioctl(fd, UI_SET_KEYBIT, KEY_I);
  ioctl(fd, UI_SET_KEYBIT, KEY_J);
  ioctl(fd, UI_SET_KEYBIT, KEY_K);
  ioctl(fd, UI_SET_KEYBIT, KEY_L);
  ioctl(fd, UI_SET_KEYBIT, KEY_M);
  ioctl(fd, UI_SET_KEYBIT, KEY_N);
  ioctl(fd, UI_SET_KEYBIT, KEY_O);
  ioctl(fd, UI_SET_KEYBIT, KEY_P);
  ioctl(fd, UI_SET_KEYBIT, KEY_Q);
  ioctl(fd, UI_SET_KEYBIT, KEY_R);
  ioctl(fd, UI_SET_KEYBIT, KEY_S);
  ioctl(fd, UI_SET_KEYBIT, KEY_T);
  ioctl(fd, UI_SET_KEYBIT, KEY_U);
  ioctl(fd, UI_SET_KEYBIT, KEY_V);
  ioctl(fd, UI_SET_KEYBIT, KEY_W);
  ioctl(fd, UI_SET_KEYBIT, KEY_X);
  ioctl(fd, UI_SET_KEYBIT, KEY_Y);
  ioctl(fd, UI_SET_KEYBIT, KEY_Z);

  // Configurando o dispositivo virtual.
  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234; /* sample vendor */
  usetup.id.product = 0x5678; /* sample product */
  strcpy(usetup.name, "Teclado Virtual");

  // Criando o dispositivo virtual de fato.
  ioctl(fd, UI_DEV_SETUP, &usetup);
  ioctl(fd, UI_DEV_CREATE);
  sleep(1);

  // Recebendo dados da interface gráfica python
  FILE *fp = popen("python3 window.py", "r");
  char buffer[128];

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    click(fd, buffer[0]);
  }

  pclose(fp);

  // Destruindo o dispositivo virtual.
  sleep(1);
  ioctl(fd, UI_DEV_DESTROY);
  close(fd);

  return 0;
}
