package com.example.ander.recyclerview.activity.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.AdapterView;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.example.ander.recyclerview.R;
import com.example.ander.recyclerview.activity.RecyclerItemClickListener;
import com.example.ander.recyclerview.activity.adapter.AdapterFilme;
import com.example.ander.recyclerview.activity.model.Filme;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private RecyclerView recyclerDados;
    private List<Filme> filmes = new ArrayList<Filme>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        recyclerDados = findViewById(R.id.rvItens);

        // Listagem de filmes
        criarFilmes();

        // Configurar adapter
        AdapterFilme adapter = new AdapterFilme(filmes);

        // Configurar RecyclerView
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getApplicationContext());
        recyclerDados.setLayoutManager(layoutManager);
        // fixa o tamanho para otimizar
        recyclerDados.setHasFixedSize(true);
        // adiciona linha separadora dos elementos
        recyclerDados.addItemDecoration(new DividerItemDecoration(this, LinearLayout.VERTICAL));

        recyclerDados.setAdapter(adapter);

        // Adicionando eventos de clique a partir de classe já estabelecida
        recyclerDados.addOnItemTouchListener(
                new RecyclerItemClickListener(
                        getApplicationContext(), recyclerDados, new RecyclerItemClickListener.OnItemClickListener() {
                    @Override
                    public void onItemClick(View view, int position) {
                        Toast.makeText(getApplicationContext(), "Clique em " + filmes.get(position).getTitulo(),Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onLongItemClick(View view, int position) {
                        Toast.makeText(getApplicationContext(), "Clique longo em " + filmes.get(position).getTitulo(),Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                    }
                }
                )
        );
    }

    public void criarFilmes(){
        filmes.add(new Filme("Homem Aranha - de volta ao lar", "2017", "Aventura"));
        filmes.add(new Filme("Mulher Maravilha", "2017", "Fantasia"));
        filmes.add(new Filme("Liga da Justiça", "2017", "Ficção"));
        filmes.add(new Filme("Capitão América - Guerra Civil", "2016", "Aventura/Ficção"));
        filmes.add(new Filme("It: a coisa", "2017", "Drama/Terror"));
        filmes.add(new Filme("Pica Pau - o filme", "2017", "Comédia/Animação"));
        filmes.add(new Filme("A múmia", "2017", "Terror"));
        filmes.add(new Filme("A bela e a fera", "2017", "Romance"));
        filmes.add(new Filme("Meu malvado favorito 3", "2017", "Comédia/Animação"));
        filmes.add(new Filme("Carros 3", "2017", "Comédia/Animação"));
    }
}
