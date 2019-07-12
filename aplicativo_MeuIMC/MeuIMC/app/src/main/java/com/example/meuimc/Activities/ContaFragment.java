package com.example.meuimc.Activities;

import android.content.DialogInterface;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.TextInputLayout;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.R;

public class ContaFragment extends Fragment {
    private String emailBD, senhaBD, nomeBD;
    private EditText etEmail, etNome, etSenha, etSenha2;
    private TextInputLayout tilEmail, tilNome, tilSenha, tilSenha2;
    private SQLiteDatabase db;
    private Button btnSalvar;
    private DBSQLiteHelper banco;
    private TextView mensagem;
    private ContaModel conta;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view =  inflater.inflate(R.layout.fragment_conta, container, false);

        //pega dados da Activity responsável por esse fragmento
        IMCActivity activity = (IMCActivity) getActivity();
        emailBD = activity.retornaEmail();
        senhaBD = activity.retornaSenha();

        finds(view);

        etEmail.setText(emailBD);
        mensagem.setText("");

        btnSalvar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                ContaModel dadosNovos = new ContaModel();
                //monta dadosNovoa a ser salvo
                dadosNovos.setEmail(emailBD);
                dadosNovos.setNome(etNome.getText().toString());
                dadosNovos.setPassword(etSenha.getText().toString());
                String senhaEt = etSenha.getText().toString();
                Integer tem_erro = 0;
                if(senhaEt.isEmpty()){
                    tilSenha.setErrorEnabled(true);
                    tilSenha.setError("Digite uma senha.");
                    tem_erro = 1;
                }
                if(!(senhaEt.equals(etSenha2.getText().toString()))){
                    tilSenha.setErrorEnabled(true);
                    tilSenha2.setErrorEnabled(true);
                    tilSenha.setError("As senhas digitadas não coincidem.");
                    tem_erro = 1;
                }
                if(tem_erro == 0){
                    String msg = "";
                    tilSenha.setErrorEnabled(false);
                    tilSenha2.setErrorEnabled(false);

                    banco.editaUser(dadosNovos);
                    //teste
                    ContaModel conta2 = banco.getUser(emailBD);

                    if(!dadosNovos.getNome().equals(nomeBD)){
                        msg = msg + "Nome alterado com sucesso.\n";
                    }
                    if(!dadosNovos.getPassword().equals(senhaBD)){
                        msg = msg + "Senha alterada com sucesso.\n";
                    }
                    if(msg.equals("")) msg = "Nenhum dado foi alterado.\n";
                    mensagem.setText(msg);
                    nomeBD = dadosNovos.getNome();
                    senhaBD = dadosNovos.getPassword();
                }
            }
        });




        return view;
    }

    @Override
    public void onResume() {
        super.onResume();
        //busca dados
        banco = new DBSQLiteHelper(getContext());
        conta = banco.getUser(emailBD);
        if(conta.getEmail() != null) {
            etNome.setText(conta.getNome());
            nomeBD = conta.getNome();
        }


    }

    void finds(View view){
        mensagem = view.findViewById(R.id.tv_msg);
        etEmail = view.findViewById(R.id.et_email);
        etSenha = view.findViewById(R.id.et_senha);
        etSenha2 = view.findViewById(R.id.et_senha2);
        etNome = view.findViewById(R.id.et_nome);
        tilEmail = view.findViewById(R.id.til_email);
        tilNome = view.findViewById(R.id.til_nome);
        tilSenha = view.findViewById(R.id.til_senha);
        tilSenha2 = view.findViewById(R.id.til_senha2);
        btnSalvar = view.findViewById(R.id.btn_salvar);
    }

}
