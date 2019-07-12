package com.example.aulanavegandoactivities;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class Main2Activity extends AppCompatActivity {

    private TextView nome, idade;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
        nome = findViewById(R.id.tvNome);
        idade = findViewById(R.id.tvIdade);

        Bundle dados = getIntent().getExtras();
        String idade2 = String.valueOf(dados.getInt("chaveIdade");
        idade.setText(idade2);

        nome.setText(dados.getString("chaveNome"));


    }
}
