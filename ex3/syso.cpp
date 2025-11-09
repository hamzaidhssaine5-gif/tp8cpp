#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Plugin {
public:
    virtual std::string nom() const = 0;
    virtual std::string traiter(const std::string& texte) = 0;
    virtual ~Plugin() {}
    static std::unique_ptr<Plugin> creerPlugin(const std::string& type);
};

class CorrecteurOrthographique : public Plugin {
public:
    std::string nom() const override { return "Correcteur Orthographique"; }
    std::string traiter(const std::string& texte) override {
        std::string res = texte;
        size_t pos = 0;
        while ((pos = res.find("teh", pos)) != std::string::npos) {
            res.replace(pos, 3, "the");
            pos += 3;
        }
        pos = 0;
        while ((pos = res.find("recieve", pos)) != std::string::npos) {
            res.replace(pos, 7, "receive");
            pos += 7;
        }
        return res;
    }
};

class TraducteurAnglais : public Plugin {
public:
    std::string nom() const override { return "Traducteur Anglais"; }
    std::string traiter(const std::string& texte) override {
        std::string res = texte;
        struct Dico { std::string f,a; };
        Dico d[] = { {"bonjour","hello"}, {"monde","world"}, {"texte","text"}, {"test","test"}, {"exemple","example"} };
        for (auto& t : d) {
            size_t pos = 0;
            while ((pos = res.find(t.f, pos)) != std::string::npos) {
                res.replace(pos, t.f.size(), t.a);
                pos += t.a.size();
            }
        }
        return res;
    }
};

class AnalyseurStyle : public Plugin {
public:
    std::string nom() const override { return "Analyseur de Style"; }
    std::string traiter(const std::string& texte) override {
        int longues = 0;
        size_t debut = 0;
        for (size_t i=0; i<texte.size(); ++i) {
            if (texte[i]=='.' || texte[i]=='!' || texte[i]=='?') {
                if (i - debut > 100) longues++;
                debut = i+1;
            }
        }
        if (texte.size() - debut > 100) longues++;
        std::string res = texte;
        if (longues > 0) res += "\n[Analyseur de Style] " + std::to_string(longues) + " phrase(s) trop longue(s).";
        else res += "\n[Analyseur de Style] Style correct.";
        return res;
    }
};

std::unique_ptr<Plugin> Plugin::creerPlugin(const std::string& type) {
    if (type == "correcteur") return std::make_unique<CorrecteurOrthographique>();
    if (type == "traducteur") return std::make_unique<TraducteurAnglais>();
    if (type == "analyseur") return std::make_unique<AnalyseurStyle>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Plugin>> plugins;
    plugins.push_back(Plugin::creerPlugin("correcteur"));
    plugins.push_back(Plugin::creerPlugin("traducteur"));
    plugins.push_back(Plugin::creerPlugin("analyseur"));

    std::cout << "Entrez un texte :\n";
    std::string texte;
    std::getline(std::cin, texte);

    for (auto& p : plugins) {
        texte = p->traiter(texte);
        std::cout << "\nAprès plugin " << p->nom() << " :\n" << texte << "\n";
    }
    return 0;
}
