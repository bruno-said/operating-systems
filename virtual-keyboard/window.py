import sys
import tkinter as tk

def keyPress(key):
  print(key)
  sys.stdout.flush()

def main():
  root = tk.Tk()
  root.overrideredirect(True)
  root.title("Teclado Virtual Linux")

  for i in range(2):
    for j in range(13):
      num = 13 * i + j
      key = chr(ord('A') + num)
      button = tk.Button(root, text=f"{key}", command=lambda x=key: keyPress(x), width=5)
      button.grid(row=i, column=j)

  tk.Button(root, text="SAIR", command=quit, width=10).grid(row=2, column=13)
  tk.Button(root, text="ENTER", command=lambda: keyPress("1"), width=10).grid(row=1, column=13)
  tk.Button(root, text="ESPAÇO", command=lambda: keyPress("0")).grid(row=2, column=0, columnspan=13, sticky="nsew")
  tk.Button(root, text="APAGAR", command=lambda: keyPress("2"), width=10).grid(row=0, column=13)

  root.mainloop()

main()