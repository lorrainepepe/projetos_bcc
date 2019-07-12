package com.example.aula5_tratarerros;

import android.graphics.Color;
import android.support.design.widget.TextInputEditText;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private EditText editText_Alcool, editText_Gasol;
    private Button  btnCalc;
    private TextView tw_resultado;
    private TextInputLayout txInpAlcool, txInpGasol;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText_Alcool = findViewById(R.id.editText_Alcool);
        editText_Gasol = findViewById(R.id.editText_gasol);
        btnCalc = findViewById(R.id.btn_calc);
        txInpAlcool = findViewById(R.id.textInputLayout2);
        txInpGasol = findViewById(R.id.textInputLayout4);
        tw_resultado = findViewById(R.id.tw_result);

        btnCalc.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                System.out.println("dasjd");
                if(possuiErros() == false){
                    System.out.println("kajsd");
                    txInpAlcool.setErrorEnabled(false);
                    txInpGasol.setErrorEnabled(false);
                    Double alcool = Double.parseDouble(editText_Alcool.getText().toString());
                    Double gasol = Double.parseDouble(editText_Gasol.getText().toString());
                    Double calc = alcool/gasol;
                    System.out.println(alcool+"alcool");
                    System.out.println(gasol+"gasol");
                    System.out.println(calc+"calc");
                    if(calc >= 0.7){
                        tw_resultado.setText("Gasolina está compensando mais!");
                    } else{
                        tw_resultado.setText("Álcool está compensando mais!");
                    }
                }
            }
        });

    }
    protected boolean possuiErros(){

        if(editText_Gasol.getText().toString().length() == 0){
            txInpGasol.setErrorEnabled(true);
            txInpGasol.setError("Preencha o campo!");
            return true;
        }
        if(editText_Alcool.getText().toString().length() == 0){
            txInpAlcool.setErrorEnabled(true);
            txInpAlcool.setError("Preencha o campo!");
            return true;
        }
        return false;
    }

}
