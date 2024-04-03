#include <iostream>
#include <vector>
using namespace std;

class TrabajadorSalud{
protected:
    char* Nombre;
    char* CI;
    double salario;
public:
    //Agrege este constructor porque despues usa un constructor con estos parametros y no estaba creado
    explicit TrabajadorSalud(char* Nombre, char* CI, double salario){
        this->Nombre = Nombre;
        this->CI = CI;
        this->salario = salario;
    }

    TrabajadorSalud(){
    Nombre = new char [30];
    CI = new char [11];
    salario = 0;
    }

    virtual double calcAport()=0;

    const string& getCI() const{
        return CI;
    }

    const string& getNombre() const{
        return Nombre;
    }

    ~TrabajadorSalud(){
        delete Nombre;
        delete CI;
    }
};


class Cuadro: public TrabajadorSalud {
   private:
    char* cargo;
   public:
    Cuadro (char* Nombre, char* CI,double salario, char* cargo) : TrabajadorSalud (Nombre,CI,salario), cargo(cargo){}

    double calcAport() override {
        return 0.40*(0.15*salario);
    }

};

class Tecnico: public TrabajadorSalud {
public:
    explicit Tecnico (char*Nombre, char*CI,double salario, char*cargo) : TrabajadorSalud(Nombre,CI,salario),cargo(cargo){}
    double calcAport()override {return 0.23*(0.10*salario);}
private:
    char* cargo;
};

class Medico: public TrabajadorSalud {
public:
    Medico (char*Nombre, char*CI,double salario,double sal_acum, int cant_meses) : TrabajadorSalud(Nombre,CI,salario),sal_acum(sal_acum),cant_meses(cant_meses){}
    double calcAport()override {return 0.09*(0.05*(sal_acum/cant_meses));}
private:
    double sal_acum;
    int cant_meses;
};

class Hospital {
private:
    vector<TrabajadorSalud*> trabajadores;
public:
    void agregarTrabajador(TrabajadorSalud* nuevoTrabajador){
    trabajadores.push_back(nuevoTrabajador);
    }

    void mostrarAportesSeguridadSocial() const{
    for (const auto& trabajador: trabajadores) {
        cout << "Nombre: " << trabajador->getNombre() << "CI: " << trabajador->getCI() << "Aporte: $" << trabajador->calcAport() <<endl;
     }
    }

    void DarBaja (const string& ID){
    for (auto it = trabajadores.begin(); it != trabajadores.end(); ++it){
     if ((*it)->getCI() == ID){
        delete *it;
        trabajadores.erase(it);
        return;
     }
    }
     cerr << "Error: Trabajador no encontrado";
    }

    double CalcAportTotal() const{
    double total = 0.0;
    for (const auto& trabajador: trabajadores){
        total+= trabajador -> calcAport();
    }
    return total;
    }
    double AporteTrabajador (const string& ID) const{
    for (const auto& trabajador: trabajadores){
        if (trabajador->getCI()== ID){
            return trabajador->calcAport();
        }

      }
      cerr << "Trabajador no encontrado"<< endl;
      return 0.0;
    }

  ~Hospital(){
  for (auto TrabajadorSalud: trabajadores){
    delete TrabajadorSalud;
  }
  }
};



int main()
{
 Hospital Clinica;
char opcion;

do{
        cout << "Seleccione una opcion:" << endl;
        cout << "1. Agregar trabajador" << endl;
        cout << "2. Dar de baja a un trabajador" << endl;
        cout << "3. Calcular el aporte total a la seguridad social" << endl;
        cout << "4. Calcular el aporte de un trabajador especifico" << endl;
        cout << "5. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case '1': {
                char nombre[30], CI[12], cargo[20];
                double salario;
                int tipo_trabajador;

                cout << "Ingrese el nombre del trabajador: ";
                cin >> nombre;
                cout << "Ingrese el CI del trabajador: ";
                cin >> CI;
                cout << "Ingrese el salario del trabajador: ";
                cin >> salario;
                cout << "Ingrese el cargo del trabajador: ";
                cin >> cargo;
                cout << "Seleccione el tipo de trabajador (1. Cuadro, 2. Tecnico, 3. Medico: ";
                cin >> tipo_trabajador;

                TrabajadorSalud* nuevoTrabajador;
                switch (tipo_trabajador) {
                    case 1:
                        nuevoTrabajador = new Cuadro(nombre, CI, salario, cargo);
                        break;
                    case 2:
                        nuevoTrabajador = new Tecnico(nombre, CI, salario, cargo);
                        break;
                    case 3:
                        double sal_acum;
                        int cant_meses;
                        cout << "Ingrese el salario acumulado del trabajador: ";
                        cin >> sal_acum;
                        cout << "Ingrese la cantidad de meses trabajados: ";
                        cin >> cant_meses;
                        nuevoTrabajador = new Medico(nombre, CI, salario, sal_acum, cant_meses);
                        break;
                    default:
                        cerr << "Opcion no valida." << endl;
                        continue;
                }

                Clinica.agregarTrabajador(nuevoTrabajador);
                break;
            }
            case '2': {
                string CI;
                cout << "Ingrese el CI del trabajador a dar de baja: ";
                cin >> CI;
                Clinica.DarBaja(CI);
                break;
            }
            case '3':
                cout << "El aporte total a la seguridad social del hospital es: $" << Clinica.CalcAportTotal() << endl;
                break;
            case '4': {
                string CI;
                cout << "Ingrese el CI del trabajador para calcular su aporte: ";
                cin >> CI;
                double aporte = Clinica.AporteTrabajador(CI);
                cout << "El aporte del trabajador con el CI " << CI << ": $" << aporte << endl;
                break;
            }
            case '5':
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cerr << "Opcion no valida." << endl;
        }
    } while (opcion != '5');

    return 0;
}
