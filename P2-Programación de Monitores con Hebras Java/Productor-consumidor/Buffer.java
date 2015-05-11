import monitor.*;

class Buffer extends AbstractMonitor{
    private int numSlots = 0, cont = 0;
    private double[] buffer = null;
    private Condition buffer_con_espacio = makeCondition();
    private Condition buffer_con_datos = makeCondition();

    public Buffer (int p_numSlots){
        numSlots = p_numSlots;
        buffer = new double[numSlots];
    }

    public void depositar(double valor){
        enter();

        while(cont == numSlots){
            buffer_con_espacio.await();
        }

        buffer[cont] = valor;
        cont++;

        buffer_con_datos.signal();

        leave();
    }

    public double extraer(){
        double valor;
        enter();

        while (cont == 0){
            buffer_con_datos.await();
        }

        cont--;
        valor = buffer[cont];

        buffer_con_espacio.signal();

        leave();

        return valor;
    }
}

