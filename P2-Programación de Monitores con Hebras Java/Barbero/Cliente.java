class Cliente implements Runnable{
    public Thread thr;
    Barberia barberia;

    public Cliente(int id_cliente, Barberia p_barberia){
        barberia = p_barberia;

        thr = new Thread(this, "Cliente " + id_cliente);
    }

    public void run(){
        while (true){
            barberia.cortarPelo(); // el cliente espera (si procede) y se corta el pelo
            aux.dormir_max(2000); // el cliente está fuera de la barberia un tiempo
        }
    }
}

