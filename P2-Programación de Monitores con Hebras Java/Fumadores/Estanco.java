import monitor.AbstractMonitor;
import monitor.Condition;

class Estanco extends AbstractMonitor{
    int ingrediente;
    private Condition papel = makeCondition();
    private Condition tabaco = makeCondition();
    private Condition cerilla = makeCondition();
    private Condition sin_ingrediente = makeCondition();

    public Estanco(){
        ingrediente = -1;
    }

    // invocado por cada fumador, indicando su ingrediente o numero
    public void obtenerIngrediente(int miIngrediente){
        String ingrediente_txt="";

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

        enter();



        while(ingrediente != miIngrediente){
            switch(miIngrediente){
                case 0:
                    papel.await();
                    break;
                case 1:
                    tabaco.await();
                    break;
                case 2:
                    cerilla.await();
                    break;
            }
        }

        System.out.println("Recojo " + ingrediente_txt);
        ingrediente = -1;

        sin_ingrediente.signal();

        leave();
    }

    // invocado por el estanquero, indicando el ingrediente que pone
    public void ponerIngrediente(int ingrediente){
        enter();

        this.ingrediente = ingrediente;

        switch(ingrediente){
            case 0:
                System.out.println("Coloco papel");
                papel.signal();
                break;
            case 1:
                System.out.println("Coloco tabaco");
                tabaco.signal();
                break;
            case 2:
                System.out.println("Coloco cerilla");
                cerilla.signal();
                break;
        }

        leave();
    }

    // invocado por el estanquero
    public void esperarRecogidaIngrediente(){
        enter();

        System.out.println("Espero la recogida.");
        while(ingrediente != -1){
            sin_ingrediente.await();
        }

        leave();

    }
}

