package com.example.aula4;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.CheckBox;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    TextView valor;
    RadioGroup sabores;
    CheckBox borda, milho, bacon;
    private int total = 0;
    private int pizza = 0;
    private int pizza_anterior = 0;
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sabores = findViewById(R.id.rb_group_sabores);
        borda = findViewById(R.id.cb_borda);
        milho = findViewById(R.id.cb_milho);
        bacon = findViewById(R.id.cb_bacon);
        valor = findViewById(R.id.tw_valor);


        sabores.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if (checkedId == R.id.rb_sabor_mucarela) {
                        pizza = 20;
                }
                if (checkedId == R.id.rb_sabor_frango) {
                        pizza = 30;
                }
                if (checkedId == R.id.rb_sabor_calabresa) {
                        pizza = 25;
                }
                if (checkedId == R.id.rb_sabor_portuguesa) {
                        pizza = 30;

                }
                total = total + pizza - pizza_anterior;
                valor.setText("Total: R$"+total+",00");
                pizza_anterior = pizza;
            };
        });

        borda.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(borda.isChecked()){
                    total += 8;
                    valor.setText("Total: R$"+total+",00");
                } else{
                    total -= 8;
                    valor.setText("Total: R$"+total+",00");
                }
            }
        });
        milho.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(milho.isChecked()){
                    total +=3;
                    valor.setText("Total: R$"+total+",00");
                } else{
                    total -= 3;
                    valor.setText("Total: R$"+total+",00");
                }
            }
        });
        bacon.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(bacon.isChecked()){
                   total +=5;
                    valor.setText("Total: R$"+total+",00");
                } else{
                    total -= 5;
                    valor.setText("Total: R$"+total+",00");
                }
            }
        });

    }
}
