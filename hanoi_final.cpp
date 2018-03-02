#include <iostream>
#include <fstream>

using namespace std;

void hanoi_towers(int quantity, int from, int to, int buf_peg,int* moves,ofstream &output)
{
        if (quantity != 0)
        {
                hanoi_towers(quantity-1, from, buf_peg, to, moves,output);

                cout << from << " " << to << endl;
                output << from << " " << to << endl;
                 *moves+=1;

                hanoi_towers(quantity-1, buf_peg, to, from, moves,output);

        }

}


void pre_min(int quantity, int pegs, int* moves,ofstream &output)
{
        for(int i=2;i<quantity+2;i++)
        {
            if(i!=3)
            {
                cout <<"1 "<<i<<endl;
                output<<"1 "<<i<<endl;
                *moves+=1;
            }
        }
}

void post_min(int quantity,int pegs,int* moves,ofstream &output)
{
        for(int i=quantity+1;i>1;i--)
        {
            if(i!=3)
            {
                cout <<i<<" 3"<<endl;
                output <<i<<" 3"<<endl;
                *moves+=1;
            }
        }
}

void pre(int quantity, int pegs, int* moves, int* levels, int* lastfull,ofstream &output)
{
    int i=0;
    int licznik=quantity;

    for(i=pegs;i>3;i--)   //wyliczam, ile bede mial i wiez z (i-1) krazkami i pamietam pierwszy pelny od poczatku
    {
        if(licznik>=i-1)
        {
            levels[i]=i-1;
            licznik-=i-1;
            *lastfull=i;
        }
    }
    for(i=pegs;i>=*lastfull;i--)
    {
        if(levels[i]>0)
        {
            for(int j=2;j<2+levels[i];j++) //rozrzucam po wiezach
            {
                cout <<"1 "<<j<<endl;
                output <<"1 "<<j<<endl;
                *moves+=1;
            }
            for(int j=i-1;j>1;j--)  //zbieram
            {
                cout <<j<<" "<<i<<endl;
                output <<"1 "<<j<<endl;
                *moves+=1;
            }
        }
    }
}


void post(int quantity,int pegs,int* moves,int* levels,int* lastfull,ofstream& output)
{
    for(int i=*lastfull;i<=pegs;i++)
    {
        if(levels[i]>0)
        {
            for(int j=1;j<=levels[i];j++) //rozrzucam
            {
                if(j!=3)
                {
                    cout <<i<<" "<<j<<endl;
                    output <<i<<" "<<j<<endl;
                    *moves+=1;
                }
            }
            for(int j=i;j>0;j--) //zbieram na wiezy docelowej
            {
                if(j!=3)
                {
                cout <<j<<" 3"<<endl;
                output <<j<<" 3"<<endl;
                *moves+=1;
                }
            }
        }
    }
}

int oblicz_stala(int quantity, int pegs, int *levels) //wyliczenie stalej do przekazania do funkcji, jest suma odlozonych krazkow
{
    int wynik=0;
    for(int i=pegs;i>3;i--)
    {
        wynik+=*(levels+i);
    }

    return wynik;

}

int oblicz_stala_min(int quantity) //wyliczenie stalej do przekazania do funkcji_min
{
    return quantity-1;
}


int main()
{
        ofstream output;
        output.open("wyniki.txt",ios::app);
        int start_peg=1, destination_peg=3, buffer_peg=2, plate_quantity,pegs_quantity;
        cout << "Podaj ilosc krazkow (k>0)" << endl;
        cin  >> plate_quantity;
        cout << "Podaj ilosc wiez (n>2)"<<endl;
        cin >>  pegs_quantity;
        output<<"Dla "<<pegs_quantity<<" wiezy i "<<plate_quantity<<" krazkow"<<endl;

        int moves = 0;
        int lastfull=0;
        int *levels = new int[pegs_quantity+1];
        for(int i=0;i<pegs_quantity+1;i++){levels[i]=0;}

        if(pegs_quantity<3 || plate_quantity<0)
        {
            cout <<"Niepoprawne dane"<<endl;
        }
        else
        {
            if(pegs_quantity==3 || plate_quantity<=2)
            {
                hanoi_towers(plate_quantity, start_peg, destination_peg, buffer_peg,&moves,output);
            }
            else
            {
                if(plate_quantity<pegs_quantity)
                {
                    pre_min(plate_quantity,pegs_quantity,&moves,output);
                    int stala_min = oblicz_stala_min(plate_quantity);
                    hanoi_towers(plate_quantity-stala_min, start_peg, destination_peg, buffer_peg,&moves,output);
                    post_min(plate_quantity,pegs_quantity,&moves,output);
                }
                else
                {
                        pre(plate_quantity,pegs_quantity,&moves,levels,&lastfull,output);
                        int stala = oblicz_stala(plate_quantity,pegs_quantity,levels);
                        hanoi_towers(plate_quantity-stala, start_peg, destination_peg, buffer_peg,&moves,output);
                        post(plate_quantity,pegs_quantity,&moves,levels,&lastfull,output);
                }

            }
        }

        cout << moves <<" ruchow wykonano"<<endl;
        output << moves <<" ruchow wykonano"<<endl<<endl;

        delete []levels;

    output.close();

return 0;
}

