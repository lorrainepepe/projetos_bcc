package com.example.ander.recyclerview.activity.adapter;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.ander.recyclerview.R;
import com.example.ander.recyclerview.activity.model.Filme;

import java.util.ArrayList;
import java.util.List;
import java.util.zip.Inflater;

public class AdapterFilme extends RecyclerView.Adapter<AdapterFilme.MyViewHolder> {
    private List<Filme> listaFilmes;
    public AdapterFilme(List<Filme> listaFilmes) {
        this.listaFilmes = new ArrayList<Filme>();
        this.listaFilmes = listaFilmes;
    }

    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        // Este método cria a View para serem exibidos os elementos

        // Converte o layout XML para uma View
        View listaItens = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.adapter_lista, viewGroup,false);

        return new MyViewHolder(listaItens);
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder myViewHolder, int i) {
        // Este método atualiza a visualização e mostra os elementos
        // i representa cada posição no RecyclerView
        // Como setamos a quantidade de elementos para lista.size()
        // i vai de 0 a lista.size

        myViewHolder.titulo.setText(listaFilmes.get(i).getTitulo());
        myViewHolder.genero.setText(listaFilmes.get(i).getGenero());
        myViewHolder.ano.setText(listaFilmes.get(i).getAno());
    }

    @Override
    public int getItemCount() {
        // Retorna a quantidade de itens que serão exibidos
        return listaFilmes.size();
    }

    public class MyViewHolder extends RecyclerView.ViewHolder{
        //cria elementos gráficos que estarão no modelo
        TextView titulo;
        TextView ano;
        TextView genero;

        public MyViewHolder(@NonNull View itemView) {
            super(itemView);

            //linka os elementos do layout aos atributos da classe
            titulo = itemView.findViewById(R.id.tvTitulo);
            ano = itemView.findViewById(R.id.tvAno);
            genero = itemView.findViewById(R.id.tvGenero);
        }
    }
}
