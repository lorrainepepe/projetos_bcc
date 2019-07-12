package com.example.meuimc.Model;

public class HistoricoModel {
    private String data;
    private Double imc;
    private String classificacao, risco;
    private Double altura;
    private Double peso;
    private String email;

    public HistoricoModel(String email, String data, Double peso, Double altura) {
        this.email = email;
        this.data = data;
        setImc(peso, altura);
        this.altura = altura;
        this.peso = peso;
        setClassificacao();
        setRisco();
    }

    public HistoricoModel(String data, Double peso, Double altura) {
        this.data = data;
        setImc(peso, altura);
        this.altura = altura;
        this.peso = peso;
        setClassificacao();
        setRisco();
    }

    public HistoricoModel() {
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getRisco() {
        return risco;
    }

    public void setRisco() {
        Double imc = this.imc;
        if(imc < 18.5) this.risco ="Elevado";
        else if(imc >= 18.5 && imc <= 24.99) this.risco =  "Inexistente";
        else if(imc >= 25.00 && imc <= 29.99) this.risco = "Elevado";
        else if(imc >= 30.00 && imc <= 34.99) this.risco = "Muito elevado";
        else if(imc >= 35.00 && imc <= 39.99) this.risco = "Muitíssimo elevado";
        else this.risco = "Obseidade mórbida";

    }

    public String getClassificacao() {
        return classificacao;
    }

    public void setClassificacao() {
        Double imc = this.imc;
        if(imc < 18.5) this.classificacao = "Abaixo do peso";
        else if(imc >= 18.5 && imc <= 24.99) this.classificacao =  "Peso ideal";
        else if(imc >= 25.00 && imc <= 29.99) this.classificacao = "Excesso de Peso";
        else if(imc >= 30.00 && imc <= 34.99) this.classificacao = "Obesidade de grau 1";
        else if(imc >= 35.00 && imc <= 39.99) this.classificacao = "Obesidade de grau 2";
        else this.classificacao = "Obseidade de grau 3";
    }

    public String getData() {
        return data;
    }

    public void setData(String data) {
        this.data = data;
    }

    public Double getImc() {
        return imc;
    }

    public void setImc(Double peso, Double altura) {
        this.imc = peso/(altura*altura);
    }

    public Double getAltura() {
        return altura;
    }

    public void setAltura(Double altura) {
        this.altura = altura;
    }

    public Double getPeso() {
        return peso;
    }

    public void setPeso(Double peso) {
        this.peso = peso;
    }
}
