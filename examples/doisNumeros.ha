// Linguagem Headache
// visite https://github.com/LucasMW/Headache 
// para mais informações!
void main() {
    short a,b;
    @"Insira dois numeros\n";
    read a;
    read b;
    @"Inseridos: "; @a; @" e "; @b; @"\n";
    if(a>b) {
        @"o primeiro eh maior\n";
    } else {
        if(b>a){
            @"o segundo eh maior\n";
        } else {
            @"iguais\n";
        }
    }
    @"media: "; @(a+b) / (2 as short); @"\n";
    @"produto: "; @a*b; @"\n";
}