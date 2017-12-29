public class Main{
    public static void main(String [] args){
        Estanco estanco = new Estanco();
        Estanquero estanquero = new Estanquero(estanco);
        Fumador[]  fumadores = new Fumador[3];

        // crear hebras
        for(int i = 0; i < fumadores.length; i++)
            fumadores[i] = new Fumador(i, estanco);

        // poner en marcha las hebras
        estanquero.thr.start();

        for(int i = 0; i < fumadores.length; i++)
            fumadores[i].thr.start();
  }
}

