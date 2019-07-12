package com.example.pedrapapeloutesoura;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.Random;

public class MainActivity extends AppCompatActivity {
    private ImageView pedra, papel, tesoura, img_pc;
    private TextView resultado;
    private int escolha_user;

    public void joga(int escolha_user) {
        Random random = new Random();
        int escolha_pc = random.nextInt(3);
        if (escolha_pc == 0) img_pc.setImageResource(R.drawable.pedra);
        else if (escolha_pc == 1) img_pc.setImageResource(R.drawable.papel);
        else img_pc.setImageResource(R.drawable.tesoura);

        if (escolha_pc == escolha_user) {
            resultado.setText("Empate!!");
        } else if ((escolha_pc == 0 && escolha_user == 1) || (escolha_pc == 1 && escolha_user == 2) || (escolha_pc == 2 && escolha_user == 0)) {
            resultado.setText("Você ganhou!!");
        } else {
            resultado.setText("Você perdeu!!");
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pedra = (ImageView) findViewById(R.id.img_pedra);
        papel = (ImageView) findViewById(R.id.img_papel);
        tesoura = (ImageView)findViewById(R.id.img_tesoura);
        img_pc = (ImageView)findViewById(R.id.img_escolha_pc);
        resultado = (TextView)findViewById(R.id.tw_resultado);


        pedra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                escolha_user = 0;
                joga(escolha_user);
            }
        });
        papel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                escolha_user = 1;
                joga(escolha_user);
            }
        });
        tesoura.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                escolha_user = 2;
                joga(escolha_user);
            }
        });

    }
}
