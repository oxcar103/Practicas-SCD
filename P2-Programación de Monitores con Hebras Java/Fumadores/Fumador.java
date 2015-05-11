class Fumador implements Runnable{
    int miIngrediente;
    String ingrediente_txt;
    public Thread thr;
    Estanco estanco;

    public Fumador(int p_miIngrediente, Estanco p_estanco){
        ingrediente_txt="";

        miIngrediente = p_miIngrediente;
        estanco = p_estanco;

        switch(miIngrediente){
            case 0:
                ingrediente_txt = "papel";
                break;
            case 1:
                ingrediente_txt = "tabaco";
                break;
            case 2:
                ingrediente_txt = "cerilla";
                break;  
        }

        thr = new Thread(this, "Fumador sin " + ingrediente_txt);
    }

    public void run(){
        while(true){
            System.out.println("Quiero "+ ingrediente_txt);

            estanco.obtenerIngrediente(miIngrediente);

            System.out.println("Fumo");
  
            aux.dormir_max(2000);

            System.out.println("Termino de fumar");
        }
    }
}
