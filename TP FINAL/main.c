#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define DEMANDA "demanda.bin"
#define RECETAS "recetas.bin"
#define STOCK "stockingredientes.bin"
#define STOCKVENTA "stockventa.bin"
#define PRECIOS "precios.bin"
#define VENTAS "ventas.bin"

typedef struct
{
    char nombre_ingrediente[20];
    float cantidad;  //en kg o en litro segun el tipo de ingrediente
    char tipo[20]; /// "liquido" "solido"
    float costo; ///costo por kg o por litro según corresponda
} StockIngrediente;

typedef struct
{
    char nombre_preparacion[20];
    int cantidad; ///por unidad, no hay por peso
} Preparacion;

typedef struct
{
    char nombre_ingrediente[20];
    float cantidad; ///puede ser en litro o en kg
} IngredienteXReceta;

typedef struct
{
    char nombre_preparacion[20];
    IngredienteXReceta ingredientes[20]; ///Puede tener hasta 20 ingredientes
    int cantIngredientes; ///los validos de ingredientes
} Receta;

typedef struct
{
    char nombre_preparacion[20];
    int cantidad; ///por unidad, no hay por peso

} PreparacionVenta;

typedef struct
{
    char nombre_preparacion[20];
    float precio_venta; ///precio por unidad
} PrecioPreparacion;

typedef struct
{
    char nombre_preparacion[20];
    int cantidad;
    float valor;
} PedidoPreparacion;

typedef struct
{
    int idVenta;
    PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
    int cantItems; ///los validos del arreglo de items_pedido
    float valor_total; ///valor total a pagar
    int bajaLogica;
} Venta;
//Paso 1
void depersistenciadelosarchivos(StockIngrediente[],int*,Receta[],int*,Preparacion[],int*);
void depersistirstock(StockIngrediente[],int*);
void depersistirreceta(Receta[],int*);
void depersistirdemanda(Preparacion[],int*);
//Paso 2
void Paso2();
void preparaciones(Receta[],int,Preparacion[],int,StockIngrediente[],int, PreparacionVenta[]);
void ubicaciondemandapreparada(Receta[],int,Preparacion[],int,int[]);
void cantdeproduccion(StockIngrediente[],Receta[],int,int,int,int,int*);
void restastock(StockIngrediente[],Receta[],int[],int,int,int[],int);
void sobreescribirstock(StockIngrediente[],int);
void preparadosparalaventa(PreparacionVenta[],int);
void cantpreparados(PreparacionVenta[],Preparacion[],int,int[]);
void mostrarTICKET(Venta[],int);
void gotoxy(int,int);
//Paso 3
void preciospreparados(StockIngrediente[],Receta[],int,int,int);
void persistenciaPrecioPreparacion(PrecioPreparacion[],int);
void costodelospreparados(StockIngrediente[],Receta[],int,int,float[]);
void depersistenciastockdepreps(PreparacionVenta[],int*);
void restastkven(PreparacionVenta[],int,int*);
void ubiprecistckven(PreparacionVenta[],int,PrecioPreparacion[],int,int[]);
float precioapagar(int,int,PrecioPreparacion[]);
void precioprodu(float*,float*,int,int,PrecioPreparacion[]);
void persistirVentas(Venta[],int);
void despersistirPrecios(PrecioPreparacion[],int*);
void comprausuario(PreparacionVenta[],PrecioPreparacion[],int,int,Venta*,int*);
void mostrarTicket(Venta);
void cargaproductos(Venta*,PreparacionVenta[],int,float,int*,int);
void cargaaventas(Venta*,int,float,int);
void stckvensobrescritura(PreparacionVenta[],int);
void estaventascargado(Venta[],int*);
//Paso 4
void vendedor();
void comprador();
void menu();
void mostrarStock();
void inicializador();
void mostrarListPreparados();
void depersistirventas(Venta[],int*);
void ingreso(float*,int);
void ganancia();
void cambiarprecio();
void aumento(PrecioPreparacion[],Receta[],int,StockIngrediente[],int,float[]);
void descuento(PrecioPreparacion[],Receta[],int,StockIngrediente[],int,float[]);
void bajaLogica(Venta[],int);
void devolucion();
void recargastkventa(Venta[],int);
void costodelospreparadosaumento(StockIngrediente[],Receta[],int,int,float[],float);
void costodelospreparadosdescuento(StockIngrediente[],Receta[],int,int,float[],float);
void aumentoporingrediente(StockIngrediente[],int,int,float);
void descuentoporingrediente(StockIngrediente[],int,int,float);

void Paso2()
{
    StockIngrediente astock[30];
    int vstock=0;
    Receta arecetas[30];
    int vrecetas=0;
    Preparacion ademanda[30];
    int vdemanda=0;
    PreparacionVenta aprepventa[vdemanda];

    depersistenciadelosarchivos(astock,&vstock,arecetas,&vrecetas,ademanda,&vdemanda);
    preparaciones(arecetas,vrecetas,ademanda,vdemanda,astock,vstock,aprepventa);
    preciospreparados(astock,arecetas,vrecetas,vstock,0);

    sobreescribirstock(astock,vstock);
}

void depersistirstock(StockIngrediente arreglo[],int* validos)
{
    int i=(*validos);
    FILE* fp=fopen(STOCK,"rb");
    if(fp !=NULL)
    {
        while(fread(&arreglo[i],sizeof(StockIngrediente),1,fp)>0)
        {
            i++;
        }
        (*validos)=i;

        fclose(fp);
    }
}

void depersistirreceta(Receta arreglo[],int* validos)
{
    int i=(*validos);
    FILE* fp=fopen(RECETAS,"rb");
    if(fp !=NULL)
    {
        while(fread(&arreglo[i],sizeof(Receta),1,fp)>0)
        {
            i++;
        }
        (*validos)=i;

        fclose(fp);
    }
}

void depersistirdemanda(Preparacion arreglo[],int* validos)
{
    int i=(*validos);
    FILE* fp=fopen(DEMANDA,"rb");
    if(fp !=NULL)
    {
        while(fread(&arreglo[i],sizeof(Preparacion),1,fp)>0)
        {
            i++;
        }
        (*validos)=i;

        fclose(fp);
    }
}

void persistirVentas(Venta arreglo[],int validos)
{
    FILE *fp=fopen(VENTAS,"ab");
    if(fp!=NULL)
    {
        fwrite(arreglo,sizeof(Venta),1,fp);

        fclose(fp);
    }
}

void depersistenciadelosarchivos(StockIngrediente astock[],int* vstock,Receta areceta[],int* vreceta,Preparacion ademanda[],int* vdemanda)
{
    depersistirstock(astock,vstock);
    depersistirreceta(areceta,vreceta);
    depersistirdemanda(ademanda,vdemanda);
}

void preparaciones(Receta arecetas[],int vrecetas,Preparacion ademanda[],int vdemanda,StockIngrediente astock[],int vstock, PreparacionVenta aprepventa[])
{
    int h=0;
    int produccion=1000;
    int cantproducida[vdemanda];
    int auxh[20];
    int auxj[20];


    int posicionrecetasdemandas[vdemanda];
    ubicaciondemandapreparada(arecetas,vrecetas,ademanda,vdemanda,posicionrecetasdemandas);

    for(int i=0; i<vdemanda; i++)
    {
        for(int j=0; j<arecetas[posicionrecetasdemandas[i]].cantIngredientes; j++)
        {
            while(strcmpi(arecetas[posicionrecetasdemandas[i]].ingredientes[j].nombre_ingrediente,astock[h].nombre_ingrediente) != 0 && h<vstock)
            {
                h++;
            }
            cantdeproduccion(astock,arecetas,ademanda[i].cantidad,posicionrecetasdemandas[i],j,h,&produccion);
            auxh[j]=h;
            auxj[j]=j;
            h=0;
        }
        cantproducida[i]=produccion;
        restastock(astock,arecetas,auxh,posicionrecetasdemandas[i],arecetas[posicionrecetasdemandas[i]].cantIngredientes,auxj,produccion);
        produccion=1000;
    }
    cantpreparados(aprepventa,ademanda,vdemanda,cantproducida);
}

void ubicaciondemandapreparada(Receta arecetas[],int vrecetas,Preparacion ademanda[],int vdemanda,int posicionrecetas[])
{
    int j=0;

    for(int i= 0; i<vdemanda; i++)
    {
        while(strcmpi(ademanda[i].nombre_preparacion,arecetas[j].nombre_preparacion)!=0 && j<vrecetas)
        {
            j++;
        }
        posicionrecetas[i]=j;
        j=0;
    }
}

void cantdeproduccion(StockIngrediente astock[],Receta areceta[],int cantdemand,int posicioningreceta,int ingredientereceta,int ingredientestock,int* cantidadpreparada)
{
    int i=0;
    float j= astock[ingredientestock].cantidad;
    float h= areceta[posicioningreceta].ingredientes[ingredientereceta].cantidad;

    while(j>=h && i<cantdemand)
    {
        j-=h;
        i++;
    }
    if(i<(*cantidadpreparada))
    {
        (*cantidadpreparada)=i;
    }
}

void restastock(StockIngrediente astock[],Receta areceta[],int posingrstock[],int posireceta,int cantingrece,int posingrece[],int cantpreparada)
{
    for(int i=0; i<cantpreparada; i++)
    {
        for(int j=0; j<cantingrece; j++)
        {
            astock[posingrstock[j]].cantidad-=areceta[posireceta].ingredientes[posingrece[j]].cantidad;
        }
    }
}

void cantpreparados(PreparacionVenta aprepventa[],Preparacion ademanda[],int vdemanda,int cantproducida[])
{
    for(int i=0; i<vdemanda; i++)
    {
        strcpy(aprepventa[i].nombre_preparacion,ademanda[i].nombre_preparacion);
        (aprepventa[i].cantidad)=(cantproducida[i]);
    }
    if(cantproducida[0]==10)
    {
        preparadosparalaventa(aprepventa,vdemanda);
    }
}

void sobreescribirstock(StockIngrediente astock[],int vstock)
{
    FILE* fp=fopen(STOCK,"wb");
    if(fp!=NULL)
    {
        fwrite(astock,sizeof(StockIngrediente),vstock,fp);
        fclose(fp);
    }
}

void preparadosparalaventa(PreparacionVenta aprepventa[],int validosdemanda)
{
    FILE* fp=fopen(STOCKVENTA,"wb");
    if(fp!=NULL)
    {
        fwrite(aprepventa,sizeof(Preparacion),validosdemanda,fp);
        fclose(fp);
    }
}

void preciospreparados(StockIngrediente astock[],Receta areceta[],int vreceta,int vstock,int entrada)
{
    PrecioPreparacion aprecios[vreceta];
    float costotprep[vreceta];

    if(entrada==1)
    {
        char pregunta;
        gotoxy (3,5);
        printf("Desea hacer un aumento o una descuento del precio? A[aumento] o D[descuento]\n");
        gotoxy (3,6);
        fflush(stdin);
        scanf("%c",&pregunta);

        system("pause");
        system("cls");

        if(pregunta == 'A' || pregunta == 'a')
        {
            aumento(aprecios,areceta,vreceta,astock,vstock,costotprep);
        }
        else if(pregunta == 'D' || pregunta == 'd')
        {
            descuento(aprecios,areceta,vreceta,astock,vstock,costotprep);
        }
        else
        {
            gotoxy (3,8);
            printf("Ingrese una opcion correcta");
            system("pause");
            system("cls");
            preciospreparados(astock,areceta,vreceta,vstock,1);
        }
    }
    else
    {
        costodelospreparados(astock,areceta,vreceta,vstock,costotprep);

        for(int i=0; i<vreceta; i++)
        {
            strcpy(aprecios[i].nombre_preparacion,areceta[i].nombre_preparacion);
            aprecios[i].precio_venta=(costotprep[i]+(costotprep[i]*0.2));
        }
        persistenciaPrecioPreparacion(aprecios,vreceta);
    }
}

void costodelospreparados(StockIngrediente astock[],Receta areceta[],int vreceta,int vstock,float totalcostpreps[])
{
    int h=0;
    float costotot=0;

    for(int i=0; i<vreceta; i++)
    {
        for(int j=0; j<areceta[i].cantIngredientes; j++)
        {
            while(strcmpi(areceta[i].ingredientes[j].nombre_ingrediente,astock[h].nombre_ingrediente))
            {
                h++;
            }
            costotot+=(astock[h].costo)*(areceta[i].ingredientes[j].cantidad);
            h=0;
        }
        totalcostpreps[i]=costotot;
        costotot=0;
    }
}

void persistenciaPrecioPreparacion(PrecioPreparacion aprecios[],int validos)
{

    FILE* fp=fopen(PRECIOS,"wb");
    if(fp!=NULL)
    {
        fwrite(aprecios,sizeof(PrecioPreparacion),validos,fp);

        fclose(fp);
    }
}

void ventas()
{
    PreparacionVenta arreglostckven[30];
    int validosastckven=0;
    PrecioPreparacion aPrecios[30];
    int validosPrecios = 0;
    Venta aventas[30];
    int vventas=0;
    estaventascargado(aventas,&vventas);
    Venta x;


    FILE* fp=fopen(VENTAS,"ab");
    if(fp!=NULL)
    {
        depersistenciastockdepreps(arreglostckven,&validosastckven);
        despersistirPrecios(aPrecios,&validosPrecios);

        comprausuario(arreglostckven,aPrecios,validosastckven,validosPrecios,&x,&vventas);

        fwrite(&x,sizeof(Venta),1,fp);

        fclose(fp);
    }
}

void comprausuario(PreparacionVenta arreglostckven[],PrecioPreparacion aPrecios[],int validosastckven,int vprecio,Venta* x,int* validosventas)
{
    int posistck[validosastckven];
    int opusuario;
    int cantitems;
    int j=0;
    char seguir='s';
    float total=0;
    float precioXproducto=0;

    ubiprecistckven(arreglostckven,validosastckven,aPrecios,vprecio,posistck);

    printf("Que desea comprar?\n\n");
    do
    {
        if (j >= 1)
        {
            printf ("Que producto desea agregar?\n\n");
        }
        gotoxy (3, 2);
        printf ("Productos");
        gotoxy (40, 2);
        printf ("Precio unitario");
        gotoxy (60, 2);
        printf ("Stock");
        for(int i=0; i<validosastckven; i++)
        {
            gotoxy (3,i+4);
            printf("%i. %s\n",i+1,aPrecios[posistck[i]].nombre_preparacion);
            gotoxy (40, i+4);
            printf ("$%.2f", aPrecios[posistck[i]].precio_venta);
            gotoxy (60, i+4);
            printf ("%i", arreglostckven[i].cantidad);
        }
        gotoxy (1,validosastckven+4);
        printf("\nElija una opcion: ");
        fflush(stdin);
        scanf("%i",&opusuario);

        printf("Que cantidad desea comprar?\n");
        printf("Cantidad: ");
        fflush(stdin);
        scanf("%i",&cantitems);

        restastkven(arreglostckven,opusuario,&cantitems);
        if(cantitems>0)
        {
            precioprodu(&precioXproducto,&total,posistck[opusuario-1],cantitems,aPrecios);
            cargaproductos(x,arreglostckven,cantitems,precioXproducto,&j,opusuario);
            stckvensobrescritura(arreglostckven,opusuario);
        }

        printf ("Desea agregar mas productos? SI[s] / NO[n]\n");
        fflush (stdin);
        scanf ("%c", &seguir);
        system ("cls");
    }
    while ((seguir == 's' || seguir == 'S') && j < 20);
    if(j==20)
    {
        printf("Ha alcanzado el limite de articulos a comprar\n");
    }
    (*validosventas)++;
    cargaaventas(x,j,total,(*validosventas));

    mostrarTicket((*x));
}

void estaventascargado(Venta aventas[],int* vventas)
{
    FILE* fp=fopen(VENTAS,"rb");
    if(fp!=NULL)
    {
        if(fread(aventas,sizeof(Venta),1,fp)>0)
        {
            depersistirventas(aventas,vventas);
        }

        fclose(fp);
    }
}

void precioprodu(float* precioXproducto,float* total,int posicion,int cantitems,PrecioPreparacion aPrecios[])
{
    (*precioXproducto)=precioapagar(posicion,cantitems,aPrecios);
    (*total)+=(*precioXproducto);
}

void cargaproductos(Venta* x,PreparacionVenta arreglostckven[],int cantitems,float precioXproducto,int* contador,int opusuario)
{
    int j=(*contador);
    strcpy((*x).items_pedido[j].nombre_preparacion,arreglostckven[opusuario-1].nombre_preparacion);
    (*x).items_pedido[j].cantidad=cantitems;
    (*x).items_pedido[j].valor=precioXproducto;
    (*contador)++;
}

void ubiprecistckven(PreparacionVenta astkven[],int vstkven,PrecioPreparacion aprecio[],int vprecio,int posiigual[])
{
    int h=0;
    for(int i=0; i<vstkven; i++)
    {
        while(strcmpi(astkven[i].nombre_preparacion,aprecio[h].nombre_preparacion)!=0 && h<vprecio)
        {
            h++;
        }
        posiigual[i]=h;
        h=0;
    }
}

void cargaaventas(Venta* x,int itemssolicitados,float total,int posant)
{
    (*x).idVenta=posant;
    (*x).cantItems=itemssolicitados;
    (*x).valor_total=total;
    (*x).bajaLogica=1;
}

void mostrarTicket(Venta x)
{
    int i = 0;
    gotoxy(1,1);
    printf("ID VENTA: %i\n",x.idVenta);
    gotoxy (3, 2);
    printf ("Productos");
    gotoxy (40, 2);
    printf ("Cantidad");
    gotoxy (60, 2);
    printf ("Precio");
    for(i = 0; i < x.cantItems ; i++)
    {
        gotoxy (3,i+4);
        printf("%i. %s\n",i+1, x.items_pedido[i].nombre_preparacion);
        gotoxy (60, i+4);
        printf ("$%.2f", x.items_pedido[i].valor);
        gotoxy (40, i+4);
        printf ("%i", x.items_pedido[i].cantidad);
    }
    gotoxy (60, 22);
    printf ("$%.2f\n",x.valor_total);
    gotoxy (3, 22);
    printf ("TOTAL");
    for (int j=8; j<60;j++)
    {
        gotoxy (j, 22);
        printf (".");
    }
    printf ("\n\n");
    system ("pause");
    system ("cls");
}

void restastkven(PreparacionVenta astkven[],int opusuario,int* cantitems)
{
    int i=opusuario-1;
    int j=(*cantitems);

    if(astkven[i].cantidad>=j)
    {
        astkven[i].cantidad-=j;
    }
    else if(astkven[i].cantidad!=0)
    {
        printf("Disculpenos, el stock actual es insuficiente para satisfacer su demanda\n");
        printf("Actualmente en stock hay %i, se le entregara esa cantidad de %s\n",astkven[i].cantidad,astkven[i].nombre_preparacion);
        (*cantitems)=astkven[i].cantidad;
        astkven[i].cantidad-=j;
    }
    else
    {
        printf("No hay stock, sepa disculparnos\n");
        (*cantitems)=0;
        system("pause");
        system("cls");
    }
}

float precioapagar(int posicion,int cantidaddeitems,PrecioPreparacion aprecios[])
{
    float total=0;
    total=(aprecios[posicion].precio_venta*cantidaddeitems);
    return total;
}

void depersistenciastockdepreps(PreparacionVenta arreglo[],int* validos)
{
    int i=(*validos);
    FILE* fp=fopen(STOCKVENTA,"rb");
    if(fp !=NULL)
    {
        while(fread(&arreglo[i],sizeof(PreparacionVenta),1,fp)>0)
        {
            i++;
        }
        (*validos)=i;

        fclose(fp);
    }
}

void despersistirPrecios (PrecioPreparacion aPrecios[],int* validosPrecios)
{
    int i = 0;
    FILE* fp = fopen (PRECIOS, "rb");
    if (fp != NULL)
    {
        while(fread(&aPrecios[i],sizeof(PrecioPreparacion),1,fp)>0)
        {
            i++;
        }
        (*validosPrecios)=i;
        fclose (fp);
    }
}

void stckvensobrescritura(PreparacionVenta astckven[],int opusuario)
{
    int i=opusuario-1;
    FILE* fp=fopen(STOCKVENTA,"r+b");
    if(fp!=NULL)
    {
        fseek(fp,sizeof(PreparacionVenta)*i,SEEK_SET);
        fwrite(&astckven[i],sizeof(PreparacionVenta),1,fp);

        fclose(fp);
    }
}

void mostrarStock()
{
    StockIngrediente astock[30];
    int vstock=0;

    depersistirstock(astock,&vstock);

    gotoxy (3, 2);
    printf ("Ingredientes");
    gotoxy (40, 2);
    printf ("Precio unitario");
    gotoxy (60, 2);
    printf ("Stock");
    for(int i=0; i<vstock; i++)
    {
        gotoxy (3,i+4);
        printf("%i. %s\n",i+1,astock[i].nombre_ingrediente);
        gotoxy (40, i+4);
        printf ("$%.2f", astock[i].costo);
        gotoxy (60, i+4);
        printf ("%.2f", astock[i].cantidad);
    }
    printf ("\n");

}

void mostrarListPreparados()
{
    int h=0;
    PreparacionVenta arreglostckven[30];
    int validosastckven=0;
    PrecioPreparacion aPrecios[30];
    int vprecio=0;

    despersistirPrecios(aPrecios,&vprecio);
    depersistenciastockdepreps(arreglostckven,&validosastckven);

    gotoxy (3, 2);
    printf ("Productos");
    gotoxy (40, 2);
    printf ("Precio unitario");
    gotoxy (60, 2);
    printf ("Stock");
    for(int i=0; i<validosastckven; i++)
    {
        while(strcmpi(arreglostckven[i].nombre_preparacion,aPrecios[h].nombre_preparacion)!=0 && h<vprecio)
        {
            h++;
        }
        gotoxy (3,i+4);
        printf("%i. %s\n",i+1,aPrecios[h].nombre_preparacion);
        gotoxy (40, i+4);
        printf ("$%.2f", aPrecios[h].precio_venta);
        gotoxy (60, i+4);
        printf ("%i", arreglostckven[i].cantidad);
        h=0;
    }
    printf ("\n");
}

void depersistirventas(Venta aventas[],int* validos)
{
    int i=(*validos);

    FILE* fp=fopen(VENTAS,"rb");
    if(fp!=NULL)
    {
        while(fread(&aventas[i],sizeof(Venta),1,fp))
        {
            i++;
        }
        (*validos)=i;

        fclose(fp);
    }
}

void devolucion()
{
    Venta salvador[1];
    FILE* fp=fopen(VENTAS,"r+b");
    if(fp!=NULL)
    {
        gotoxy (45, 2);
        printf ("MENU DEVOLUCION");
        for (int i = 0; i < 15; i++)
        {
            gotoxy (45 + i, 3);
            printf ("Ä");
        }

        if(fread(&salvador[0],sizeof(Venta),1,fp)>0)
        {
            Venta aventas[30];
            int validos=0;
            int idcompra;
            int i=0;

            depersistirventas(aventas,&validos);
            gotoxy (3,5);
            printf("Ingrese el Id de su compra: ");
            fflush(stdin);
            scanf("%i",&idcompra);

            while(aventas[i].idVenta!=idcompra && i<validos)
            {
                i++;
            }

            if(i<validos)
            {
                if(aventas[i].bajaLogica!=0)
                {
                    bajaLogica(aventas,i);
                    recargastkventa(aventas,i);
                    gotoxy (3,7);
                    printf ("Se ha realizado la devolucion.\n");
                }
                else
                {
                    gotoxy (3,7);
                    printf("No se ha encontrado su compra, sera devuelto al menu.\n");
                    system("pause");
                    system("cls");
                    vendedor();
                }
            }
            else
            {
                gotoxy (3,7);
                printf("No se ha encontrado su compra, sera devuelto al menu.\n");
                system("pause");
                system("cls");
            }

        }
        fclose(fp);
    }
    else
    {
        gotoxy (3,15);
        printf("Aun no se regitran ventas\n");
    }
}

void bajaLogica(Venta arreglo[],int posicion)
{
    arreglo[posicion].bajaLogica=0;
    FILE* fp=fopen(VENTAS,"wb");
    if(fp!=NULL)
    {
        fseek(fp,sizeof(Venta)*posicion,SEEK_SET);
        fwrite(&arreglo[posicion],sizeof(Venta),1,fp);

        fclose(fp);
    }
}

void recargastkventa(Venta aventa[],int posicion)
{
    int h=0;
    PreparacionVenta astckven[30];
    int vastckven=0;

    depersistenciastockdepreps(astckven,&vastckven);

    for(int i=0;i<aventa[posicion].cantItems;i++)
    {
        while(strcmpi(aventa[posicion].items_pedido[i].nombre_preparacion,astckven[h].nombre_preparacion))
        {
            h++;
        }
        astckven[h].cantidad+=aventa[posicion].items_pedido[i].cantidad;
        h=0;
    }
    preparadosparalaventa(astckven,vastckven);
}

void mostrarlistventas()
{
    Venta aventa[30];
    int vventas=0;
    int h=0;
    int i;
    FILE* fp=fopen(VENTAS,"rb");
    if(fp!=NULL)
    {
        if(fread(&aventa[0],sizeof(Venta),1,fp)>0)
        {
            depersistirventas(aventa,&vventas);
            gotoxy(3,2);
            printf("ID VENTA");
            gotoxy(15,2);
            printf("CANT ITEMS");
            gotoxy(30,2);
            printf("LISTADO PRODUCTOS");
            gotoxy(60,2);
            printf("CANTIDAD");
            gotoxy(90,2);
            printf("VALOR");

            for(i=0; i<vventas; i++)
            {
                if(aventa[i].bajaLogica==1)
                {
                    gotoxy(3,4+h+i);
                    printf("%i ",aventa[i].idVenta);
                    gotoxy(15,4+h+i);
                    printf("%i ",aventa[i].cantItems);

                    for(int j=0; j<aventa[i].cantItems; j++)
                    {
                        gotoxy(30,4+j+h+i);
                        printf("%s ",aventa[i].items_pedido[j].nombre_preparacion);
                        gotoxy(60,4+j+h+i);
                        printf("%i",aventa[i].items_pedido[j].cantidad);
                        gotoxy(90,4+j+h+i);
                        printf("%.2f",aventa[i].items_pedido[j].valor);
                        h++;
                    }
                }
            }
            printf("\n");
            gotoxy (3,6+h);
            system("pause");
        }
        fclose(fp);
    }
    else
    {
        printf("No se registran ventas en este momento");
        system("pause");
        system("cls");
    }
}

void ingreso(float* ingresos,int entrada)
{
    Venta salvador;
    FILE* fp=fopen(VENTAS,"rb");
    if(fp!=NULL)
    {
        if(fread(&salvador,sizeof(Venta),1,fp)>0)
        {
            Venta aventasdeldia[30];
            int vventas=0;

            depersistirventas(aventasdeldia,&vventas);

            for(int i=0; i<vventas; i++)
            {
                (*ingresos)+=aventasdeldia[i].valor_total;
            }
            gotoxy (3,2);
            if(entrada==1)
            {
            gotoxy (3,21);
            printf("El ingreso total del dia de hoy fue de $%.2f\n",(*ingresos));
            gotoxy (3, 22);
            }
        }
    }
    else
    {
        gotoxy (3,21);
        printf("Aun no se regitran ventas\n");
    }
    fclose(fp);
}

void ganancia()
{
    float ganaciadiaria=0;
    float ingresos=0;

    ingreso(&ingresos,0);

    ganaciadiaria=ingresos-(ingresos*0.8);

    gotoxy (3, 22);
    printf("En el dia se genero un ganacia de $%.2f\n",ganaciadiaria);
    gotoxy (3,24);
}

void cambiarprecio()
{
    StockIngrediente astock[30];
    int vstock=0;
    Receta areceta[30];
    int vreceta=0;

    depersistirstock(astock,&vstock);
    depersistirreceta(areceta,&vreceta);

    preciospreparados(astock,areceta,vreceta,vstock,1);

    system("pause");
    system("cls");
}

void aumento(PrecioPreparacion aprecios[],Receta areceta[],int vreceta,StockIngrediente astock[],int vstock,float costotprep[])
{
    float costos;
    gotoxy (3, 8);
    printf("Ingrese el aumento en decimales: \n");
    gotoxy (3, 9);
    fflush(stdin);
    scanf("%f",&costos);

    costodelospreparadosaumento(astock,areceta,vreceta,vstock,costotprep,costos);

    for(int i=0; i<vreceta; i++)
    {
        costotprep[i]+=(costotprep[i]*costos);
    }

    for(int i=0; i<vreceta; i++)
    {
        strcpy(aprecios[i].nombre_preparacion,areceta[i].nombre_preparacion);
        aprecios[i].precio_venta=(costotprep[i]+(costotprep[i]*0.2));
    }

    persistenciaPrecioPreparacion(aprecios,vreceta);
}

void costodelospreparadosaumento(StockIngrediente astock[],Receta areceta[],int vreceta,int vstock,float totalcostpreps[],float aumentotot)
{
    int h=0;
    float costotot=0;

    for(int i=0; i<vreceta; i++)
    {
        for(int j=0; j<areceta[i].cantIngredientes; j++)
        {
            while(strcmpi(areceta[i].ingredientes[j].nombre_ingrediente,astock[h].nombre_ingrediente))
            {
                h++;
            }
            aumentoporingrediente(astock,vstock,h,aumentotot);
            costotot+=(astock[h].costo)*(areceta[i].ingredientes[j].cantidad);
            h=0;
        }
        totalcostpreps[i]=costotot;
        costotot=0;
    }
}

void aumentoporingrediente(StockIngrediente astock[],int vstock,int h,float aumentotot)
{
    astock[h].costo+=(astock[h].costo*aumentotot);
    sobreescribirstock(astock,vstock);
}

void costodelospreparadosdescuento(StockIngrediente astock[],Receta areceta[],int vreceta,int vstock,float totalcostpreps[],float descuentot)
{
    int h=0;
    float costotot=0;

    for(int i=0; i<vreceta; i++)
    {
        for(int j=0; j<areceta[i].cantIngredientes; j++)
        {
            while(strcmpi(areceta[i].ingredientes[j].nombre_ingrediente,astock[h].nombre_ingrediente))
            {
                h++;
            }
            descuentoporingrediente(astock,vstock,h,descuentot);
            costotot+=(astock[h].costo)*(areceta[i].ingredientes[j].cantidad);
            h=0;
        }
        totalcostpreps[i]=costotot;
        costotot=0;
    }
}

void descuentoporingrediente(StockIngrediente astock[],int vstock,int h,float descuentotot)
{
    astock[h].costo-=(astock[h].costo*descuentotot);
    sobreescribirstock(astock,vstock);
}

void descuento(PrecioPreparacion aprecios[],Receta areceta[],int vreceta,StockIngrediente astock[],int vstock,float costotprep[])
{
    float costos;
    gotoxy (3,8);
    printf("Ingrese el descuento en decimales: \n");
    gotoxy (3,9);
    fflush(stdin);
    scanf("%f",&costos);

    costodelospreparadosdescuento(astock,areceta,vreceta,vstock,costotprep,costos);

    for(int i=0; i<vreceta; i++)
    {
        costotprep[i]-=(costotprep[i]*costos);
    }

    for(int i=0; i<vreceta; i++)
    {
        strcpy(aprecios[i].nombre_preparacion,areceta[i].nombre_preparacion);
        aprecios[i].precio_venta=(costotprep[i]+(costotprep[i]*0.2));
    }
    persistenciaPrecioPreparacion(aprecios,vreceta);
}

void menu()
{
    int opcion;
    do
    {
        gotoxy (45, 2);
        printf ("PANADERIA TRES ARROYOS");
        for (int i = 0; i < 22; i++)
        {
            gotoxy (45 + i, 3);
            printf ("Ä");
        }
        gotoxy (3, 5);
        printf("Presione 1 si usted es vendedor \n");
        gotoxy (3, 7);
        printf("Presiones 2 si usted es comprador \n");
        gotoxy (3, 9);
        printf ("Presione 0 si desea salir del programa \n");
        gotoxy (3, 11);
        printf ("Ingrese la opcion que desee: ");
        scanf("%d",&opcion);
        system("cls");
        switch(opcion)
        {
        case 1:
            vendedor();
            break;

        case 2:
            comprador();
            break;

        case 0:
            printf ("Hasta luego!");
            break;
        default:
            gotoxy (3, 13);
            printf("Opcion incorrecta\n");
            break;
        }
    }
    while (opcion != 0);
}

void comprador()
{
    int a;
    do
    {
        gotoxy (45, 2);
        printf ("MENU COMPRADOR");
        for (int i = 0; i < 14; i++)
        {
            gotoxy (45 + i, 3);
            printf ("Ä");
        }
        gotoxy (3, 5);
        printf ("Presione 1 para ver el stock de productos y sus precios\n");
        gotoxy (3, 7);
        printf ("Presione 2 para iniciar una compra\n");
        gotoxy (3, 9);
        printf ("Presione 3 para realizar una devolucion\n");
        gotoxy (3, 11);
        printf ("Presione 0 para volver al menu principal\n");
        gotoxy (3, 13);
        printf ("Ingrese la opcion que desee: ");
        scanf ("%i", &a);
        system ("cls");
        switch (a)
        {
        case 1:
            system ("cls");
            mostrarListPreparados();
            gotoxy (3,18);
            system ("pause");
            system("cls");
            break;
        case 2:
            system ("cls");
            ventas();
            break;
        case 3:
            system ("cls");
            devolucion();
            system ("cls");
            break;
        case 0:
            gotoxy (3, 15);
            printf ("Volviendo al menu principal.\n");
            gotoxy (3, 16);
            system ("pause");
            system ("cls");
            break;
        default:
            gotoxy (3, 15);
            printf ("Opcion incorrecta.\n");
            break;
        }
    }
    while (a != 0);
}

void vendedor()
{
    float ingresos=0;

    int a;

    do
    {
        gotoxy (45, 2);
        printf ("MENU VENDEDOR");
        for (int i = 0; i < 13; i++)
        {
            gotoxy (45 + i, 3);
            printf ("Ä");
        }
        gotoxy (3, 5);
        printf("Presione 1 para ver Remanente de Stock de Ingredientes\n");
        gotoxy (3, 7);
        printf("Presione 2 para ver la Lista de Preparados y cantidad restante\n");
        gotoxy (3, 9);
        printf("Presione 3 para ver el Ingreso Total del dia\n");
        gotoxy (3, 11);
        printf("Presione 4 para ver la Ganancia del Dia \n");
        gotoxy (3, 13);
        printf("Presione 5 ver Ventas Realizadas \n");
        gotoxy (3, 15);
        printf("Presione 6 para Cambiar un Precio \n");
        gotoxy (3, 17);
        printf ("Presione 0 para volver al menu principal\n");
        gotoxy (3, 19);
        printf ("Ingrese la opcion que desee: ");
        scanf("%d",&a);
        system ("cls");

        switch (a)
        {
        case 1:
            system ("cls");
            mostrarStock();
            gotoxy (3,24);
            system ("pause");
            system ("cls");
            break;
        case 2:
            system ("cls");
            mostrarListPreparados();
            gotoxy (3,18);
            system ("pause");
            system ("cls");
            break;
        case 3:
            ingreso(&ingresos,1);
            gotoxy (3,22);
            system ("pause");
            system ("cls");
            break;
        case 4:
            ganancia();
            gotoxy (3,24);
            system ("pause");
            system ("cls");
            break;
        case 5:
            system ("cls");
            mostrarlistventas();
            system ("cls");
            break;
        case 6:
            system ("cls");
            cambiarprecio();
            gotoxy (3, 11);
            system ("pause");
            system ("cls");
            break;
        case 0:
            gotoxy (3, 21);
            printf ("Volviendo al menu principal.\n");
            gotoxy (3, 23);
            system ("pause");
            system ("cls");
            break;
        default:
            gotoxy (3, 21);
            printf ("Opcion incorrecta.");
            gotoxy (3, 22);
            system ("pause");
            system ("cls");
            break;
        }
    }
    while (a != 0);
}

void gotoxy(int x,int y)
{
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y= y;
    SetConsoleCursorPosition(hcon,dwPos);
}

void inicializador()
{
    Paso2();
    menu();
}

int main()
{
    inicializador();
    return 0;
}
