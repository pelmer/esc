// The loops in foo are not vectorized. Those in bar are
float a1[1024], a2[1024],  b[1024], c[1024], d[1024], e[1024]; bool k[1024];


void foo () {
  for (int i = 0; i < 1024; i++) if(b[i] < c[i]) a1[i]=0; 
  for (int i = 0; i < 1024; i++) a2[i] = b[i] < c[i] ? d[i] : e[i];
  for (int i = 0; i < 1024; i++) k[i] = b[i] < c[i]  && d[i] < e[i];
}


void bar (void){
  for (int i = 0; i < 1024; i++)
    a1[i] = b[i] < c[i]  ? 0 : a1[i];
  for (int i = 0; i < 1024; i++) {
    float d_ = d[i], e_ = e[i];
    a2[i] = b[i] < c[i] ? d_ : e_;
  }
  for (int i = 0; i < 1024; i++) k[i] = (b[i] < c[i]) &  (d[i] < e[i]);
}
