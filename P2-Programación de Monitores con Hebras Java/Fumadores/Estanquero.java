class Estanquero implements Runnable{
    public Thread thr;
    Estanco estanco;

    public Estanquero(Estanco p_estanco){
        estanco = p_estanco;
        thr = new Thread(this, "Estanquero");
    };

    public void run(){
        int ingrediente ;

        while (true){
            ingrediente = (int) (Math.random () * 3.0); // 0,1 o 2

            estanco.ponerIngrediente(ingrediente);
            estanco.esperarRecogidaIngrediente();
        }
    }
}

