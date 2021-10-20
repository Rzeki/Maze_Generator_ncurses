#ifndef STACK_HXX
#define STACK_HXX
#include <iostream>
#include <stdexcept>

template<class T, int n>
class Stack {

    T* tab; //miejsce w pamieci w ktorym zostanie utworzona tablica dla stosu
    unsigned int stack_size; //obecna dlugosc stosu

    public:
    Stack();  //konstruktor domyslny
    ~Stack();   //dekonstruktor
    Stack(const Stack<T,n> &obj);   //konstruktor kopiujacy
    Stack(Stack<T,n> &&obj);  //konstruktor przenoszacy
    Stack& operator=(const Stack<T,n>& obj); //kopiujacy operator 
    Stack& operator=(Stack<T,n>&& obj); //operator przeniesienia
    template<class U>
    void push(U&& x);  //uniwersalna referencja
    T pop();
    T& top();
    int size();
    bool empty();
    void show(); //wyswietlanie stosu
};

template<class T,int n>
Stack<T,n>::Stack() : tab{new T[n]}, stack_size{0} { } //konstruktor domyslny


template<class T,int n>
Stack<T,n>::Stack(Stack<T,n> &&obj) : tab{obj.tab}, stack_size(obj.stack_size) //members initializers
{        
    obj.stack_size=0;
    obj.tab = nullptr;         //konstruktor przenoszacy
}

template<class T,int n>
Stack<T,n>::Stack(const Stack<T,n> &obj) : tab{new T[n]}, stack_size{obj.stack_size}
{
    for(int i=0;i<stack_size;i++)
    {
        tab[i] = obj.tab[i];     //konstruktor kopiujacy
    }
}

template<class T,int n>
Stack<T,n>& Stack<T,n>::operator=(const Stack<T,n>& obj)
{
    if(&obj != this)
    {
        delete [] tab; //zwalnianie miejsca starej tablicy
        tab = new T[n]; //tworzenie nowej tablicy
        stack_size = obj.stack_size; //kopiowanie rozmiaru stosu
        for(int i=0;i<stack_size;i++)
        {
            tab[i] = obj.tab[i];
        }
    }
    
    return *this; //zwracam caly obiekt
}

template<class T,int n>
Stack<T,n>& Stack<T,n>::operator=(Stack<T,n>&& obj)
{
    delete [] tab; 
    tab = obj.tab;
    stack_size = obj.stack_size;
    obj.tab = nullptr;
    obj.stack_size = 0;
    return *this;
}

template<class T,int n>
Stack<T,n>::~Stack()
{
    delete[] tab;     //dekonstruktor
}

template<class T,int n>
template<class U>
void Stack<T,n>::push(U&& x)
{
    if(stack_size >= n)
    {
        throw std::out_of_range("ERROR! Stack elements are out of range!\n"); 
    }                                                                           
    else
    {
        tab[stack_size] = std::forward<U>(x);
        stack_size++;
    }  
}

template<class T,int n>
T Stack<T,n>::pop()
{
    if(empty())
    {
        throw std::out_of_range("ERROR! Stack elements are out of range!");
    }
    else 
    {
        stack_size--;
        return tab[stack_size];
    }

}

template<class T,int n>
T& Stack<T,n>::top()
{
    if(!empty())
    {
        return tab[stack_size-1];
    }
    else
    {
        throw std::out_of_range("Pointer to empty stack!");
    }
}

template<class T,int n>
int Stack<T,n>::size()
{
    return stack_size;
}

template<class T,int n>
bool Stack<T,n>::empty()
{
    return (stack_size==0);
}

template<class T,int n>
void Stack<T,n>::show()
{
    std::cout<<std::endl<<std::endl;
    std::cout<<"------------"<<std::endl;
    for(int i=stack_size-1;i>=0;i--)
    {
        std::cout<<tab[i]<<std::endl;
    }
    std::cout<<"------------"<<std::endl;
    std::cout<<std::endl<<std::endl;
}


#endif