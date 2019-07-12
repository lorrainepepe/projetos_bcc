package com.example.aula6_toast;

import android.content.DialogInterface;
import android.preference.DialogPreference;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private Button btn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn = findViewById(R.id.btn_clique);

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                geraAlert();
            }
        });
    }

    public void geraAlert(){
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Aprendendo Dialog e Toast");
        dialog.setMessage("Escolha uma opção");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_alert);

        dialog.setPositiveButton("Sim", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getApplicationContext(),"Você clicou em positivo", Toast.LENGTH_SHORT).show();
            }
        });

        dialog.setNegativeButton("Não", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Toast.makeText(getApplicationContext(),"Você clicou em negativo", Toast.LENGTH_SHORT).show();
            }
        });

        dialog.create();
        dialog.show();
    }

}

