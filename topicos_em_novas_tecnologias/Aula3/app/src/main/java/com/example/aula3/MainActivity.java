package com.example.aula3;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Random;

public class MainActivity extends AppCompatActivity {
    private Button btn_gerar_frase;
    private TextView tw_result;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn_gerar_frase = (Button) findViewById(R.id.btn_gerar_frase);
        tw_result = (TextView) findViewById(R.id.tw_result);

        btn_gerar_frase.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Random random = new Random();
                Integer numero = random.nextInt(4);
                String frase;
                switch (numero){
                    case 1:{
                        frase = "A gente tem que rebolar moçada";
                        break;
                    }
                    case 2:{
                        frase = "Tenho filho pequeno na escola";
                        break;
                    }
                    case 3:{
                        frase = "Que mandioca é essa?";
                        break;
                    }
                    default:{
                        frase = "Tá frescão hoje...";
                    }
                }

                tw_result.setText(frase);
            }
        });


    }
}
