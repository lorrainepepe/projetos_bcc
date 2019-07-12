package com.example.meuimc.Activities;

import android.content.Intent;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.ContaModel;
import com.example.meuimc.R;

public class LoginActivity extends AppCompatActivity {
    private Button entrar, esquecisenha, inscrevase;
    private EditText etEmail, etSenha;
    private TextInputLayout tilEmail, tilSenha;
    private DBSQLiteHelper db;
    private String email;
    private ContaModel conta;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        setTitle("Meu IMC - Login");

        entrar = findViewById(R.id.btn_entrar);
        esquecisenha = findViewById(R.id.btn_esqueci);
        inscrevase = findViewById(R.id.btn_inscrevase);
        etEmail = findViewById(R.id.et_email_login);
        etSenha = findViewById(R.id.et_senha);
        tilEmail = findViewById(R.id.til_email_login);
        tilSenha = findViewById(R.id.til_senha);


        etEmail.setText("");
        etSenha.setText("");

        tilEmail.setErrorEnabled(false);
        tilSenha.setErrorEnabled(false);

        entrar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //pegar dados
                String senha;
                String  email_BD, nome_BD, senha_BD = null;
                Boolean ok = false, ok2= false;
                email = etEmail.getText().toString();
                senha = etSenha.getText().toString();
                Integer tem_erro = 0;

                //Verifica se o usuário existe no banco
                db = new DBSQLiteHelper(getApplicationContext());
                if(email != null) conta = db.getUser(email);
                if(conta.getEmail() == null) ok = false;
                else ok = true;

                if(senha.equals(conta.getPassword())) ok2 = true;
                else ok2 = false;


                if(email == null || email.equals("") || email.isEmpty()){
                    tem_erro = 1;
                    tilEmail.setErrorEnabled(true);
                    tilEmail.setError("E-mail obrigatório");
                } else if(senha == null || senha.equals("") || senha.isEmpty()) {
                    tem_erro = 1;
                    tilSenha.setErrorEnabled(true);
                    tilSenha.setError("Senha obrigatória");
                } else if(ok == false){
                    tilEmail.setErrorEnabled(true);
                    tilEmail.setError("Usuário não encontrado. Entre com um usuário válido ou inscreva-se.");
                    tem_erro = 1;
                } else if(ok2 == false){
                    tilSenha.setErrorEnabled(true);
                    tilEmail.setErrorEnabled(true);
                    tilEmail.setError("Email e/ou senha não conferem.");
                    tem_erro = 1;
                } else if(tem_erro == 0){ // se não existem erros
                    tilEmail.setErrorEnabled(false);
                    tilSenha.setErrorEnabled(false);
                    Intent intent = new Intent(LoginActivity.this, IMCActivity.class);
                    intent.putExtra("email", email);
                    intent.putExtra("senha", senha);
                    startActivity(intent);
                }
//verificar erros (se não existe no banco, se não coincide email com a senha do usuario, se os campos estão vazios
            }
        });

//fazer o esqueci minha senha
        esquecisenha.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this, EsqueciSenhaActivity.class);
                startActivity(intent);
            }
        });

        inscrevase.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this, InscrevaseActivity.class);
                startActivity(intent);
            }
        });
    }

}
