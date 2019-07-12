package com.example.meuimc.Activities;

import android.content.DialogInterface;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.R;

import java.sql.SQLOutput;

public class InscrevaseActivity extends AppCompatActivity {
    private TextInputLayout tilNome, tilEmail, tilSenha, tilSenha2;
    private EditText etNome, etEmail, etSenha, etSenha2;
    private Button btnLimpar, btnCadastrar;
    private DBSQLiteHelper db;
    private Integer existe;
    private String email;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_inscrevase);
        setTitle("Meu IMC - Criar conta");

        tilEmail = findViewById(R.id.til_email);
        tilNome = findViewById(R.id.til_nome);
        tilSenha = findViewById(R.id.til_password);
        tilSenha2 = findViewById(R.id.til_password2);

        etEmail = findViewById(R.id.et_email);
        etSenha = findViewById(R.id.et_password);
        etSenha2 = findViewById(R.id.et_password2);
        etNome = findViewById(R.id.et_nome);

        btnLimpar = findViewById(R.id.btn_limpar);
        btnCadastrar = findViewById(R.id.btn_cadastrar);

        //inicializa banco
        db = new DBSQLiteHelper(this);


        btnLimpar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                etNome.setText("");
                etEmail.setText("");
                etSenha.setText("");
                etSenha2.setText("");
            }
        });

        btnCadastrar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String nome, email, senha, senha2;
                Double peso, altura;
                //Pega o que foi digitado pelo usuário nos editText
                nome = etNome.getText().toString();
                email = etEmail.getText().toString();
                senha = etSenha.getText().toString();
                senha2 = etSenha2.getText().toString();


                Integer tem_erro = 0;
                if(nome == null || nome.equals("") || nome.isEmpty()){
                    tilNome.setErrorEnabled(true);
                    tilNome.setError("Nome obrigatório");
                    tem_erro = 1;
                } else if(email == null || email.equals("") || email.isEmpty()){
                    tilEmail.setErrorEnabled(true);
                    tilEmail.setError("E-mail obrigatório");
                    tem_erro = 1;
                } else if(senha == null || senha.equals("") || senha.isEmpty()){
                    tilSenha.setErrorEnabled(true);
                    tilSenha.setError("Digite uma senha.");
                    tem_erro = 1;
                } else if(senha2 == null || senha2.equals("") || senha2.isEmpty()){
                    tilSenha2.setErrorEnabled(true);
                    tilSenha2.setError("Repetição de senha obrigatória");
                    tem_erro = 1;
                } else if(!senha.equals(senha2)){
                    tilSenha.setErrorEnabled(true);
                    tilSenha2.setErrorEnabled(true);
                    tilSenha.setError("Senha e repetição de senha não coincidem!");
                    tilSenha2.setError("Senha e repetição de senha não coincidem!");
                    tem_erro = 1;
                } else{
                    existe = 0;
                    ContaModel conta = db.getUser(email);
                    if(conta.getEmail() != null) {
                        existe = 1;
                    }

                    if(existe == 1){
                        tilEmail.setErrorEnabled(true);
                        tilEmail.setError("E-mail já cadastrado.");
                        tem_erro = 1;

                    } else if(existe == -1){
                        tem_erro = 1;
                    }
                }

                if(tem_erro == 0){ //Se não tem erros, exibir dialog
                    tilNome.setErrorEnabled(false);
                    tilEmail.setErrorEnabled(false);
                    tilSenha.setErrorEnabled(false);
                    tilSenha2.setErrorEnabled(false);

                    ContaModel userNovo = new ContaModel(email, nome, senha);
                    db.addUser(userNovo);
                    geraAlert();

                }
            }
        });


    }

    public void geraAlert() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Conta criada");
        dialog.setMessage("Cadastro realizado com sucesso.\nAcesse sua conta para utilizar o app.");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_info);
        dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                finish();
            }
        });

        dialog.create();
        dialog.show();
    }
}
