public class Main {
    public static void main(String [] args){
        if (args.length != 5){
            System.err.println("Uso: ncons nprod tambuf niterp niterc");
           return ;
        }

        // leer parametros, crear vectores y buffer intermedio
        Consumidor[] cons = new Consumidor[Integer.parseInt(args[0])];
        Productor[]  prod = new Productor[Integer.parseInt(args[1])];
        Buffer buffer = new Buffer(Integer.parseInt(args[2]));
        int iter_cons = Integer.parseInt(args[3]);
        int iter_prod = Integer.parseInt(args[4]);

        if (cons.length*iter_cons != prod.length*iter_prod){
            System.err.println("no coinciden número de items a producir con a cosumir");
            return ;
        }    

        // crear hebras
        for(int i = 0; i < cons.length; i++) 
            cons[i] = new Consumidor(buffer,iter_cons,i);
        for(int i = 0; i < prod.length; i++)
            prod[i] = new Productor(buffer,iter_prod,i);

        // poner en marcha las hebras
        for(int i = 0; i < prod.length; i++)
            prod[i].thr.start();
        for(int i = 0; i < cons.length; i++)
            cons[i].thr.start();
  }
}
