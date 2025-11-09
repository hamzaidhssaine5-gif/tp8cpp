#include <iostream>
#include <string>

class SortieAudio {
public:
    virtual void ouvrir() = 0;
    virtual void jouer(const std::string& son) = 0;
    virtual void fermer() = 0;
    virtual ~SortieAudio() {}
};

class HautParleur : public SortieAudio {
public:
    void ouvrir() override { std::cout << "HautParleur ouvert\n"; }
    void jouer(const std::string& son) override { std::cout << "HautParleur joue " << son << "\n"; }
    void fermer() override { std::cout << "HautParleur fermé\n"; }
    ~HautParleur() override {}
};

class CasqueBT : public SortieAudio {
public:
    void ouvrir() override { std::cout << "Casque Bluetooth ouvert\n"; }
    void jouer(const std::string& son) override { std::cout << "Casque Bluetooth joue " << son << "\n"; }
    void fermer() override { std::cout << "Casque Bluetooth fermé\n"; }
    ~CasqueBT() override {}
};

class SortieHDMI : public SortieAudio {
public:
    void ouvrir() override { std::cout << "Sortie HDMI ouverte\n"; }
    void jouer(const std::string& son) override { std::cout << "Sortie HDMI joue " << son << "\n"; }
    void fermer() override { std::cout << "Sortie HDMI fermée\n"; }
    ~SortieHDMI() override {}
};

void tester(SortieAudio* device) {
    device->ouvrir();
    device->jouer("son.wav");
    device->fermer();
}

int main() {
    SortieAudio* devices[3];
    devices[0] = new HautParleur();
    devices[1] = new CasqueBT();
    devices[2] = new SortieHDMI();

    for (int i = 0; i < 3; ++i) {
        tester(devices[i]);
    }

    for (int i = 0; i < 3; ++i) {
        delete devices[i];
    }
    return 0;
}
