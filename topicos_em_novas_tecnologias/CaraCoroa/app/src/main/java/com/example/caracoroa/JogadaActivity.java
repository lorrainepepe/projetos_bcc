package com.example.caracoroa;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Toast;

public class JogadaActivity extends AppCompatActivity {
    private ImageButton voltar;
    private ImageView imgRes;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jogada);

        voltar = findViewById(R.id.imgButtonvoltar);
        imgRes = findViewById(R.id.imgRes);

        voltar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        Bundle dados = getIntent().getExtras();
        if(dados.getInt("resultado")%2 == 0){
            imgRes.setImageResource(R.drawable.moeda_cara);
            Toast.makeText(getApplicationContext(),"O resultado foi cara", Toast.LENGTH_SHORT).show();
        }
        else{
            imgRes.setImageResource(R.drawable.moeda_coroa);
            Toast.makeText(getApplicationContext(),"O resultado foi coroa", Toast.LENGTH_SHORT).show();

        }


    }
}
