package com.example.aulanavegandoactivities;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    private Button btnEnvia;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnEnvia = findViewById(R.id.btn_envia);


        btnEnvia.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent (getApplicationContext(), Main2Activity.class);
                intent.putExtra("chaveNome", "Lorraine M. Pepe");
                intent.putExtra("chaveIdade", 10);

                startActivity(intent);


            }
        });



    }
}
