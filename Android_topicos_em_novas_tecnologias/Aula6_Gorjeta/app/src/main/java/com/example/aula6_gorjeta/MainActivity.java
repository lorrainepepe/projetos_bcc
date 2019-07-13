package com.example.aula6_gorjeta;

import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private TextInputLayout textInputLayout;
    private SeekBar seekBar;
    private TextView textView_total, textView_gorjeta, textView_porcent;
    private EditText editText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textInputLayout = findViewById(R.id.textInp_Compra);
        editText = findViewById(R.id.editText_compra);
        textView_gorjeta = findViewById(R.id.tw_gorjeta);
        textView_total = findViewById(R.id.tw_compra);
        textView_porcent = findViewById(R.id.tw_porcent);
        seekBar = findViewById(R.id.seekBar_porcent);

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                String texto = editText.getText().toString();
                if(texto.isEmpty()){
                    textInputLayout.setErrorEnabled(true);
                    textInputLayout.setError("Preencha com o valor da compra");
                    textView_porcent.setText(seekBar.getProgress());
                } else{
                    textInputLayout.setErrorEnabled(false);
                    Integer porcentagem = seekBar.getProgress();
                    textView_porcent.setText(porcentagem+"%");
                    Double v_compra = Double.parseDouble(editText.getText().toString());
                    Double v_gorjeta = (porcentagem/100)*v_compra;
                    Double v_total = v_compra + v_gorjeta;
                    textView_gorjeta.setText("R$ "+v_gorjeta);
                    textView_total.setText("R$ "+v_total);

                }

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });




    }
}
