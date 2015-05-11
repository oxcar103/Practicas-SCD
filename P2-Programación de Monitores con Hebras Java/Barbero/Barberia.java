import monitor.*;

class Barberia extends AbstractMonitor{
    boolean barbero_libre;
    int cola;
    private Condition cliente = makeCondition();
    private Condition barbero = makeCondition();
    private Condition nuevo_corte = makeCondition();

    public Barberia(){
        barbero_libre = true;
        cola = 0;
    }

    // invcado por los clientes para cortarse el pelo
    public void cortarPelo(){
        enter();

        cola++;

        System.out.println("Entro a la barbería, soy el " + cola + "º en la cola");

        cliente.signal();

        while(!barbero_libre){
            barbero.await();
        }

        barbero_libre = false;

        System.out.println("Entro a pelarme");
        nuevo_corte.await();

        cola--;

        barbero_libre = true;

        System.out.println("Salgo de pelarme");

        leave();
    }

    // invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
    public void siguienteCliente (){
        enter();

        while(cola == 0){
            System.out.println("Espero al siguiente cliente");
            System.out.println("Zzzzzzz");
            cliente.await();
        }

        System.out.println("Tengo un nuevo cliente");

        leave();
    }

    // invocado por el barbero para indicar que ha terminado de cortar el pelo
    public void finCliente(){
        enter();

        System.out.println("He acabado de pelar");

        nuevo_corte.signal();

        barbero.signal();

        leave();
    }
}

