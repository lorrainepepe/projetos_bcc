package com.example.aula2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.w3c.dom.Text;

import java.util.Random;

public class MainActivity extends AppCompatActivity {
    private TextView textTexto; //textView criado
    private Button buttonClicar; //botão criado
    private TextView numSort;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textTexto = (TextView) findViewById(R.id.tv_texto); //associa o objeto cirado no código com o elemento criado no layout
        buttonClicar = (Button) findViewById(R.id.bt_clicar);
        numSort = (TextView) findViewById(R.id.tv_numSort);

        //Como queremos que o botão mude a mensagem quando pressionado, precisamos criar um LISTENER para escutar quando o botão será pressionado
        buttonClicar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Aqui vai o que queremos que aconteça quando o botão for pressionado
                Random random = new Random();
                Integer numero = random.nextInt(1000);
                String text = numero.toString();
                textTexto.setText("Já temos um resultado!!");
                numSort.setText(text);
            }
        });


    }
}
