package com.example.meuimc.Activities;

import android.content.DialogInterface;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.support.v4.view.GravityCompat;
import android.support.v7.app.ActionBarDrawerToggle;
import android.view.MenuItem;
import android.support.design.widget.NavigationView;
import android.support.v4.widget.DrawerLayout;

import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.example.meuimc.Adapter.AdapterHistorico;
import com.example.meuimc.DAO.DBSQLiteHelper;
import com.example.meuimc.Model.HistoricoModel;
import com.example.meuimc.R;

import java.util.ArrayList;
import java.util.List;

public class IMCActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private String email, senha;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_imc);
        setTitle("Meu IMC");
        //Essa activity é do tipo Navigation Drawer

        //pegar dados da activity anterior
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            email = extras.getString("email");
            senha = extras.getString("senha");
        }
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        //Menu lateral
        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        NavigationView navigationView = findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        //configura fragmento inicial a ser exibido
        if(savedInstanceState == null) {
            this.setTitle("Meu IMC - Cálculo");
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new CalculoFragment()).commit();
            navigationView.setCheckedItem(R.id.nav_medir);
        }
    }

    //Abre e fecha menu
    @Override
    public void onBackPressed() {
        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.imc, menu);
        return true;
    }

    //Menu de configuração
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        //Menu do Navigation Drawer indicado por reticências na vertical
        if (id == R.id.action_excluir){
            geraAlertDelete();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    //Itens selecionados
    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();
        //Opções do Navigation Drawer.
        //Alterna os fragmentos de acordo com a opção. A esta activity é a mesma para todos.
        if (id == R.id.nav_medir) {
            this.setTitle("Meu IMC - Cálculo");
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new CalculoFragment()).commit();
        } else if (id == R.id.nav_historico) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new HistoricoFragment(email, senha)).commit();
            this.setTitle("Meu IMC - Histórico");
        } else if (id == R.id.nav_conta) {
            this.setTitle("Meu IMC - Conta");
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new ContaFragment()).commit();
        } else if(id == R.id.nav_sair){
            logout(); //Usuário sai de sua conta
        }

        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    //email e senha serão enviados aos fragmentos dessa activity
    protected String retornaEmail(){
        return email;
    }

    protected String retornaSenha(){
        return senha;
    }

    public void geraAlert() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Conta excluída");
        dialog.setMessage("Sua conta foi excluída.\n Sentiremos sua falta.");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_alert);

        dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });
        dialog.create();
        dialog.show();
    }

    public void geraAlertDelete() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setTitle("Excluir conta");
        dialog.setMessage("Tem certeza que deseja excluir sua conta?");
        dialog.setCancelable(false);
        dialog.setIcon(android.R.drawable.ic_dialog_alert);
        dialog.setPositiveButton("Sim", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                DBSQLiteHelper db = new DBSQLiteHelper(getApplicationContext());
                db.deletaUser(email);
                encerra();
            }
        });

        dialog.setNegativeButton("Não", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        });

        dialog.create();
        dialog.show();
    }
    public void logout(){
        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        startActivity(intent);
    }

    public void encerra(){
        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK);
        startActivity(intent);
    }

}
