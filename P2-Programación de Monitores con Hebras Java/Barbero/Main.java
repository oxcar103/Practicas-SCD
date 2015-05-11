public class Main{
    public static void main(String [] args){
        if (args.length != 1){
            System.err.println("Uso: nclientes");
           return ;
        }

        int nclientes = Integer.parseInt(args[0]);

        Barberia barberia = new Barberia();
        Barbero barbero = new Barbero(barberia);
        Cliente[]  clientes = new Cliente[nclientes];

        // crear hebras
        for(int i = 0; i < nclientes; i++)
            clientes[i] = new Cliente(i, barberia);

        // poner en marcha las hebras
        for(int i = 0; i < nclientes; i++)
            clientes[i].thr.start();

        barbero.thr.start();
  }
}
