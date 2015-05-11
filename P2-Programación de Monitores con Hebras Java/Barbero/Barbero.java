class Barbero implements Runnable{
    public Thread thr;
    Barberia barberia;

    public Barbero(Barberia p_barberia){
        barberia = p_barberia;
        thr = new Thread(this, "Barbero");
    };

    public void run(){
        while (true){
            barberia.siguienteCliente();
            aux.dormir_max(2500); // el barbero está cortando el pelo
            barberia.finCliente();
        }
    }
}

