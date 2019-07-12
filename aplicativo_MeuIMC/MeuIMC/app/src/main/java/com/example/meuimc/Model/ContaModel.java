package com.example.meuimc.Model;

public class ContaModel {
    private String nome;
    private String email;
    private String password;

    public ContaModel(String email, String nome,String password) {
        this.nome = nome;
        this.email = email;
        this.password = password;
    }

    public ContaModel() {
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}