package com.example.meuimc.Activities;

import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.TextInputLayout;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.HistoricoModel;
import com.example.meuimc.R;

import java.text.SimpleDateFormat;

public class CalculoFragment extends Fragment {

    private SQLiteDatabase db;
    private String email, senha;
    private TextView classific, risco, imc;
    private Button calcular;
    private TextInputLayout tilPeso, tilAltura;
    private EditText etPeso, etAltura;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_calculo, container, false);

            getActivity().setTitle("Meu IMC - Cálculo");
            //as próximas três linhas pegam dados da Activity responsável por esse fragmento (IMCActivity)
            IMCActivity activity = (IMCActivity) getActivity();
            email = activity.retornaEmail();
            senha = activity.retornaSenha();

            calcular = view.findViewById(R.id.btn_calcular);
            tilAltura = view.findViewById(R.id.til_altura);
            tilPeso = view.findViewById(R.id.til_peso);
            etAltura = view.findViewById(R.id.et_altura);
            etPeso = view.findViewById(R.id.et_peso);
            classific = view.findViewById(R.id.tv_classific);
            risco = view.findViewById(R.id.tv_risco);
            imc = view.findViewById(R.id.tv_imc);

            imc.setText("");
            calcular.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Integer tem_erro = 0;
                    Double peso, altura;
                    peso = Double.parseDouble(etPeso.getText().toString());
                    altura = Double.parseDouble(etAltura.getText().toString());

                    if(etPeso.getText().toString().isEmpty()){
                        tilPeso.setErrorEnabled(true);
                        tilPeso.setError("Digite um peso válido.");
                        tem_erro = 1;
                    } else if(etAltura.getText().toString().isEmpty()){
                        tilPeso.setErrorEnabled(true);
                        tilPeso.setError("Digite uma altura válida.");
                        tem_erro = 1;
                    } else if(peso > 500){
                        tilPeso.setErrorEnabled(true);
                        tilPeso.setError("Peso deve ser em Kg.");
                        tem_erro = 1;
                    } else if(altura > 3) {
                        tilAltura.setErrorEnabled(true);
                        tilAltura.setError("Altura deve estar em m.");
                        tem_erro = 1;
                    }
                    //Se não houve erro
                    if(tem_erro == 0){
                        tilAltura.setErrorEnabled(false);
                        tilPeso.setErrorEnabled(false);

                        //Esconder o teclado virtual automaticamente para visualizar o cálculo.
                        InputMethodManager imm = (InputMethodManager) getActivity().getSystemService(IMCActivity.INPUT_METHOD_SERVICE);
                        imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);

                        Double calculo = peso/(altura*altura);
                        String i = String.format("%.2f", calculo);
                        imc.setText(i);

                        String r, c;
                        if(calculo < 18.5){
                            r ="Elevado";
                            c = "Abaixo do peso";
                            risco.setText(r);
                            risco.setTextColor(getResources().getColor(R.color.c2));
                            classific.setTextColor(getResources().getColor(R.color.c2));
                            classific.setText(c);
                        }
                        else if(calculo >= 18.5 && calculo <= 24.99){
                            r =  "Inexistente";
                            c = "Peso ideal";
                            risco.setText(r);
                            risco.setTextColor(getResources().getColor(R.color.c1));
                            classific.setTextColor(getResources().getColor(R.color.c1));
                            classific.setText(c);
                        }
                        else if(calculo >= 25.00 && calculo <= 29.99){
                            r = "Elevado";
                            c = "Excesso de Peso";
                            risco.setText(r);
                            classific.setText(c);
                            risco.setTextColor(getResources().getColor(R.color.c2));
                            classific.setTextColor(getResources().getColor(R.color.c2));
                        }
                        else if(calculo >= 30.00 && calculo <= 34.99){
                            r = "Muito elevado";
                            c ="Obesidade de grau 1";
                            risco.setText(r);
                            classific.setText(c);
                            risco.setTextColor(getResources().getColor(R.color.c3));
                            classific.setTextColor(getResources().getColor(R.color.c3));
                        }
                        else if(calculo >= 35.00 && calculo <= 39.99){
                            r = "Muitíssimo elevado";
                            c = "Obesidade de grau 2";
                            risco.setText(r);
                            classific.setText(c);
                            risco.setTextColor(getResources().getColor(R.color.c4));
                            classific.setTextColor(getResources().getColor(R.color.c4));
                        }
                        else{
                            r = "Obseidade mórbida";
                            c = "Obseidade de grau 3";
                            risco.setText(r);
                            classific.setText(c);
                            risco.setTextColor(getResources().getColor(R.color.c5));
                            classific.setTextColor(getResources().getColor(R.color.c5));
                        }
                        //Pega a data atual
                        SimpleDateFormat formatador = new SimpleDateFormat("dd/MM/yyyy");
                        long data = System.currentTimeMillis();
                        String dataString = formatador.format(data);
                        //Salva o cálculo no histórico do usuário
                        DBSQLiteHelper bd = new DBSQLiteHelper(getContext());
                        HistoricoModel h = new HistoricoModel(email, dataString, peso, altura);
                        bd.addItemHistorico(h);
                    }
                }
            });
        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        //Limpa classificação e risco quando esse fragmento é exibido.
        classific.setText("");
        risco.setText("");

    }
}
